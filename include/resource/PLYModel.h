#pragma once

#include <string>
#include <vector>
#include <stdint.h>
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
};

struct Face
{
    int32_t indices_count;
    std::vector<int32_t> indices;
};

struct Edge
{
    int32_t vertex1;
    int32_t vertex2;
    glm::vec3 color;
};

struct PLYModel
{
    bool is_triangulated;

    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    std::vector<Edge> edges;

    PLYModel();
    ~PLYModel() = default;

    void Triangulate();
};