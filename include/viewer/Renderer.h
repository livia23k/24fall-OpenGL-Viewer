#pragma once
#include "resource/PLYModel.h"
#include "camera/Camera.h"

#include <glm/glm.hpp>
#include <glad/glad.h>

struct Renderer
{
    Renderer();
    ~Renderer();

    void Initialize();
    void Clean();

    void EnableDepthTestSetting();

    void UploadModel(const PLYModel &model);
    void UploadTransformMatrix();

    void MakeCameraFocusOnModel(const PLYModel &target_model);
    
    void Render();

private:
    GLuint vao;              // Vertex Array Object
    GLuint vbo;              // Vertex Buffer Object
    GLuint ebo;              // Element Buffer Object for faces
    size_t num_face_indices; // Number of face indices

    GLuint shader_program;
    Camera camera;

    std::string LoadShaderSource(const std::string &filePath);    
    void CompileShader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);
};