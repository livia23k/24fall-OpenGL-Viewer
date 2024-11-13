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
    int edge_count = 0;
    bool is_reading_header = true;

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
            else if (token == "edge")
            {
                ss >> edge_count;
            }
        }
        else if (token == "end_header")
        {
            is_reading_header = false;
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

        ss >> vertex.position.x >> vertex.position.y >> vertex.position.z;
        ss >> r >> g >> b;

        vertex.color = glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f); // Normalize color to [0,1]
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

    // Load edges
    for (int i = 0; i < edge_count; ++i)
    {
        if (!std::getline(file, line))
        {
            std::cerr << "Error: Unexpected end of file while reading edges." << std::endl;
            return false;
        }
        std::istringstream ss(line);
        Edge edge;
        int r, g, b;

        ss >> edge.vertex1 >> edge.vertex2;
        ss >> r >> g >> b;

        edge.color = glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f); // Normalize color to [0,1]
        model.edges.push_back(edge);
    }

    models.push_back(model);

    file.close();
    return true;
}