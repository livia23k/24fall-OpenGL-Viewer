#pragma once
#include "datatype/BBox.h"

#include <string>
#include <vector>
#include <stdint.h>
#include <glm/glm.hpp>
#include <glad/glad.h>

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

struct PLYModel
{
    std::string name;
    std::string type;
    bool is_triangulated;
    bool should_render;
    BBox bbox;

    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    std::vector<Vertex> vertices;
    std::vector<Face> faces;

    PLYModel();
    ~PLYModel() = default;

    void Triangulate();
};