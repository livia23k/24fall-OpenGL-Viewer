#include "PLYMgr.h"

#include <iostream>
#include <fstream>
#include <sstream>

bool PLYMgr::LoadPLY(const std::string &filepath)
{
    PLYModel model;

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

        model.bbox.enclose(vertex.position);

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
        
        model.vertices.push_back(vertex);
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
        model.faces.push_back(face);
    }

    // Emit edges
    // ...

    // Model info
    model.name = std::filesystem::path(filepath).filename().string();
    model.type = "Object";

    // Save model
    models.push_back(model);

    file.close();
    return true;
}