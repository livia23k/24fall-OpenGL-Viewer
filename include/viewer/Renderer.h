#pragma once
#include "resource/PLYModel.h"
#include "resource/PLYMgr.h"
#include "camera/Camera.h"

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <unordered_map>

struct Renderer
{
public:
    Camera camera;

    Renderer();
    ~Renderer();

    void Initialize();
    void Clean();

    void UploadModel(const std::shared_ptr<PLYModel> &model);
    void UploadTransformMatrix();

    void MakeCameraFocusOnModel(const PLYModel &target_model);
    void SetView(const std::string& view_type, const PLYModel& target_model);
    
    void Render(const PLYMgr &plyManager);

private:
    size_t num_face_indices; // Number of face indices
    
    std::unordered_map<std::shared_ptr<PLYModel>, GLuint> vao_map;
    std::unordered_map<std::shared_ptr<PLYModel>, GLuint> vbo_map;
    std::unordered_map<std::shared_ptr<PLYModel>, GLuint> ebo_map;

    GLuint shader_program;

    std::string LoadShaderSource(const std::string &filePath);    
    void CompileShader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);
};