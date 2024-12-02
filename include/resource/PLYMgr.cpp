#include "PLYMgr.h"

#include <iostream>
#include <fstream>
#include <sstream>

void PLYMgr::ComputeNormals(const std::shared_ptr<PLYModel>& model)
{
    if (!model)
    {
        std::cerr << "Error: Model is null. Cannot compute normals." << std::endl;
        return;
    }

    // Initialize normals
    for (auto &vertex : model->vertices)
    {
        vertex.normal = glm::vec3(0.0f);
    }
    // Compute face normals and accumulate them for each vertex
    for (const auto &face : model->faces)
    {
        if (face.indices.size() < 3)
        {
            std::cerr << "Warning: Face with less than 3 vertices encountered. Skipping." << std::endl;
            continue;
        }

        // Get the positions of the first three vertices in the face
        const glm::vec3 &v0 = model->vertices[face.indices[0]].position;
        const glm::vec3 &v1 = model->vertices[face.indices[1]].position;
        const glm::vec3 &v2 = model->vertices[face.indices[2]].position;

        // Compute the face normal using the cross product of two edges
        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

        // Accumulate the face normal to each vertex normal of the face
        for (int32_t index : face.indices)
        {
            model->vertices[index].normal += faceNormal;
        }
    }

    // Normalize the accumulated normals for each vertex
    for (auto &vertex : model->vertices)
    {
        vertex.normal = glm::normalize(vertex.normal);
    }

    std::cout << "Normals computed successfully for model: " << model->name << std::endl;
}


bool PLYMgr::LoadPLY(const std::string &filepath)
{
    auto model = std::make_shared<PLYModel>();

    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filepath << std::endl;
        return false;
    }

    std::string line;
    int vertex_count = 0;
    int face_count = 0;
    bool is_reading_header = true;
    bool has_rgb_info = false;
    bool has_normal_info = false;

    // Parse the header
    while (is_reading_header && std::getline(file, line))
    {
        std::istringstream ss(line);
        std::string token;
        ss >> token;

        if (token == "element")
        {
            ss >> token;
            if (token == "vertex")
            {
                ss >> vertex_count;
            }
            else if (token == "face")
            {
                ss >> face_count;
            }
        }
        else if (token == "end_header")
        {
            is_reading_header = false;
        }
        else if (token == "property")
        {
            ss >> token;
            if (token == "uchar")
            {
                ss >> token;
                if (token == "red")
                {
                    has_rgb_info = true;
                }
            }
            else if (token == "double")
            {
                ss >> token;
                if (token == "nx")
                {
                    has_normal_info = true;
                }
            }
        }
    }

    // Load vertices
    for (int i = 0; i < vertex_count; ++i)
    {
        if (!std::getline(file, line))
        {
            std::cerr << "Error: Unexpected end of file while reading vertices." << std::endl;
            return false;
        }
        std::istringstream ss(line);
        Vertex vertex;
        int r, g, b;
        float nx, ny, nz;

        model->bbox.enclose(vertex.position);

        if (vertex.position.y > 10000) {
            std::cerr << "Incorrect vertex position: " << vertex.position.x << ", " << vertex.position.y << vertex.position.z << std::endl;
        }

        ss >> vertex.position.x >> vertex.position.y >> vertex.position.z;

        if (has_normal_info) // TOCHECK: pass the normal info
        {
            ss >> nx >> ny >> nz;
        }

        if (has_rgb_info)
        {
            ss >> r >> g >> b;
            vertex.color = glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f); // Normalize color to [0,1]
        }
        else
        {
            vertex.color = glm::vec3(0.0f, 1.0f, 0.0f); // Pre-defined color: Green
        }
        
        model->vertices.push_back(vertex);
    }

    // Load faces
    for (int i = 0; i < face_count; ++i)
    {
        if (!std::getline(file, line))
        {
            std::cerr << "Error: Unexpected end of file while reading faces." << std::endl;
            return false;
        }
        std::istringstream ss(line);
        Face face;
        int vertex_count_in_face;

        ss >> vertex_count_in_face;
        for (int j = 0; j < vertex_count_in_face; ++j)
        {
            int vertex_index;
            ss >> vertex_index;
            face.indices.push_back(vertex_index);
        }
        model->faces.push_back(face);
    }

    // Emit edges
    // ...

    // Compute Normals
    ComputeNormals(model);

    // Model info
    model->name = std::filesystem::path(filepath).filename().string();
    model->type = "Object";

    // Save model
    models.push_back(model);

    file.close();
    return true;
}