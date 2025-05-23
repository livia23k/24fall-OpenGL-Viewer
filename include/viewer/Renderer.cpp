#include "viewer/Renderer.h"
#include "resource/PLYMgr.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer() 
    : camera(), 
      num_face_indices(0), 
      shader_program(0) 
{
    // Initialize OpenGL container maps
    vao_map.clear();
    vbo_map.clear();
    ebo_map.clear();
}

Renderer::~Renderer()
{
    Clean();
}

void Renderer::Initialize()
{
    CompileShader("shader/vertex_shader.glsl", "shader/fragment_shader.glsl");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::cout << "[Renderer] Renderer initialized successfully." << std::endl;
}

void Renderer::Clean()
{
    for (auto &[model, vao] : vao_map)
    {
        glDeleteVertexArrays(1, &vao);
    }
    for (auto &[model, vbo] : vbo_map)
    {
        glDeleteBuffers(1, &vbo);
    }
    for (auto &[model, ebo] : ebo_map)
    {
        glDeleteBuffers(1, &ebo);
    }

    vao_map.clear();
    vbo_map.clear();
    ebo_map.clear();

    if (shader_program != 0)
    {
        glDeleteProgram(shader_program);
        shader_program = 0;
    }
}

std::string Renderer::LoadShaderSource(const std::string &file_path)
{
    std::string source;
    std::ifstream shader_file;
    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        shader_file.open(file_path);
        std::stringstream shader_stream;
        shader_stream << shader_file.rdbuf();
        shader_file.close();
        source = shader_stream.str();

    }
    catch(const std::exception& e)
    {
        std::cerr << "Failed to open shader file: " << file_path << "\n";
        std::cerr << "Reason: " << e.what() << std::endl;
    }

    return source;
}

void Renderer::CompileShader(const std::string &vert_path, const std::string &frag_path)
{
    const std::string vert_source = LoadShaderSource(vert_path);
    const std::string frag_source = LoadShaderSource(frag_path);

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    const char *vertex_shader_source = vert_source.c_str();
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertex_shader);

    GLint success;
    char info_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
        std::cerr << "Failed to compile vertex shader: " << info_log << std::endl;
    }

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragment_shader_source = frag_source.c_str();
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
        std::cerr << "Failed to compile fragment shader: " << info_log << std::endl;
    }

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_program, 512, nullptr, info_log);
        std::cerr << "Failed to link shader program: " << info_log << std::endl;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Renderer::UploadModel(const std::shared_ptr<PLYModel> &model)
{
    if (!model)
    {
        std::cerr << "Error: Model is null. Cannot upload data." << std::endl;
        return;
    }

    GLuint vao, vbo, ebo;

    // Prepare vertex data
    std::vector<float> vertex_data;
    for (const auto &vertex : model->vertices)
    {
        vertex_data.push_back(vertex.position.x);
        vertex_data.push_back(vertex.position.y);
        vertex_data.push_back(vertex.position.z);

        vertex_data.push_back(vertex.color.r);
        vertex_data.push_back(vertex.color.g);
        vertex_data.push_back(vertex.color.b);

        vertex_data.push_back(vertex.normal.x);
        vertex_data.push_back(vertex.normal.y);
        vertex_data.push_back(vertex.normal.z);

    }

    // Prepare face indices
    std::vector<GLuint> face_indices;
    for (const auto &face : model->faces)
    {
        face_indices.insert(face_indices.end(), face.indices.begin(), face.indices.end());
    }
    num_face_indices = face_indices.size();

    // Generate VAO, VBO, and EBO for the model
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // Upload vertex data
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(float), vertex_data.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Upload face indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_indices.size() * sizeof(GLuint), face_indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    // Store VAO, VBO, and EBO in the maps
    vao_map[model] = vao;
    vbo_map[model] = vbo;
    ebo_map[model] = ebo;
}

void Renderer::UploadTransformMatrix()
{
    glm::mat4 transform = camera.get_perspective_matrix();
    unsigned int transformLoc = glGetUniformLocation(shader_program, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
}

void Renderer::MakeCameraFocusOnModel(const PLYModel &target_model)
{
    glm::vec3 model_center = target_model.transformed_bbox.center();
    float model_radius = glm::length(target_model.transformed_bbox.max - target_model.transformed_bbox.min) * 0.5f;
    camera.look_at_model(model_center, model_radius);
}

void Renderer::Render(const PLYMgr &plyManager)
{
    glUseProgram(shader_program);
    UploadTransformMatrix();

    for (const auto &model : plyManager.models)
    {
        if (vao_map.find(model) == vao_map.end())
        {
            std::cerr << "Error: VAO not found for model" << std::endl;
            continue;
        }

        if (!model->should_render)
        {
            continue;
        }

        GLuint modelLoc = glGetUniformLocation(shader_program, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model->model_matrix));

        glBindVertexArray(vao_map[model]);
        glDrawElements(GL_TRIANGLES, model->faces.size() * 3, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}

void Renderer::SetView(const std::string& view_type, const PLYModel& target_model)
{
    glm::vec3 target = target_model.transformed_bbox.center(); // Get the center of the model
    float radius = glm::length(target_model.transformed_bbox.max - target_model.transformed_bbox.min) * 0.5f; // Estimate model size
    float distance = radius * 2.0f; // Adjust the camera distance based on the model size

    if (view_type == "Top")
    {
        camera.SetTopView(target, distance);
    }
    else if (view_type == "Front")
    {
        camera.SetFrontView(target, distance);
    }
    else if (view_type == "Side")
    {
        camera.SetSideView(target, distance);
    }
    else
    {
        std::cerr << "Unknown view type: " << view_type << std::endl;
    }
}
