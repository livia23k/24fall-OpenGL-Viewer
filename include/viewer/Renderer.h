#pragma once
#include "resource/PLYModel.h"

#include <glm/glm.hpp>
#include <glad/glad.h>

struct Renderer
{
    Renderer();
    ~Renderer();

    void Initialize();
    void Clean();

    void UploadModel(const PLYModel &model);

    void Render();

private:
    GLuint vao;              // Vertex Array Object
    GLuint vbo;              // Vertex Buffer Object
    GLuint ebo;              // Element Buffer Object for faces
    GLuint edge_ebo;         // Element Buffer Object for edges
    size_t num_face_indices; // Number of face indices
    size_t num_edge_indices; // Number of edge indices

    GLuint shader_program;

    std::string LoadShaderSource(const std::string &filePath);
    void CompileShader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);
};