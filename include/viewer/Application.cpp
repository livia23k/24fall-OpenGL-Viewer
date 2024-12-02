#include "viewer/Application.h"
#include "resource/PLYMgr.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "tinyfd/tinyfiledialogs.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

struct ImGuiWindowData
{
    // TODO:
};

void Application::RenderObjectLibraryPanel()
{
    ImGui::Begin("Object Library & File Management");

    if (ImGui::Button("Import Model"))
    {
        const char* filePath = tinyfd_openFileDialog(
            "Choose a PLY Model",       // Dialog title
            "",                         // Default path
            1,                          // Number of file filters
            (const char*[]){ "*.ply" }, // Filters
            "PLY Files (*.ply)",        // Description
            0                           // Allow multiple selection (0 = false)
        );

        if (filePath)
        {
            if (plyManager.LoadPLY(filePath))
            {
                renderer.UploadModel(plyManager.models.back());
                renderer.MakeCameraFocusOnModel(*(plyManager.models.back()));
                windowManager.ResetMouseCenter();
                renderer.camera.reset_eular_angles();

                current_focusing_model = plyManager.models.back();
            }
            else
            {
                std::cerr << "Failed to load PLY model: " << filePath << std::endl;
            }
        }
    }

    ImGui::SeparatorText("Loaded Models");
    for (const auto& model : plyManager.models)
    {
        ImGui::Text("Name: %s", model->name.c_str());
        ImGui::Text("Type: %s", model->type.c_str());

        ImGui::Checkbox(("Render " + model->name).c_str(), &(model->should_render));
        
        if (ImGui::Button(("Focus on " + model->name).c_str()))
        {
            renderer.MakeCameraFocusOnModel(*model);
            windowManager.ResetMouseCenter();
            renderer.camera.reset_eular_angles();

            current_focusing_model = model;
        }

        ImGui::Separator();
    }

    ImGui::End();
}

void Application::RenderCameraAndViewControls()
{
    ImGui::Begin("Camera and View Controls");

    ImGui::SeparatorText("Camera Movement");
    if (ImGui::Button("Reset View"))
    {
        renderer.MakeCameraFocusOnModel(*(current_focusing_model));
        windowManager.ResetMouseCenter();
        renderer.camera.reset_eular_angles();
    }

    ImGui::Text("Quick View Presets");
    if (ImGui::Button("Top View"))
    {
        renderer.SetView("Top", *current_focusing_model); 
        windowManager.ResetMouseCenter();
    }

    if (ImGui::Button("Front View"))
    {
        renderer.SetView("Front", *current_focusing_model);
        windowManager.ResetMouseCenter();
    }

    if (ImGui::Button("Side View"))
    {
        renderer.SetView("Side", *current_focusing_model);
        windowManager.ResetMouseCenter();
    }

    ImGui::End();
}

void Application::RenderObjectManipulationPanel()
{
    if (!current_focusing_model)
        return;

    ImGui::Begin("Object Manipulation & Properties");

    ImGui::SeparatorText("Item Details");
    ImGui::Text("Name: %s", current_focusing_model->name.c_str());
    ImGui::Text("Manufacturer: TBD");
    ImGui::Text("Price: TBD");
    ImGui::Text("Size: TBD");

    ImGui::SeparatorText("Model Transform");

    if (ImGui::SliderFloat3("Translation", glm::value_ptr(current_focusing_model->translation), -10.0f, 10.0f))
    {
        UpdateModelMatrix(current_focusing_model);
    }

    if (ImGui::SliderFloat3("Rotation", glm::value_ptr(current_focusing_model->rotation), -180.0f, 180.0f))
    {
        UpdateModelMatrix(current_focusing_model);
    }

    if (ImGui::SliderFloat3("Scaling", glm::value_ptr(current_focusing_model->scaling), 0.1f, 10.0f))
    {
        UpdateModelMatrix(current_focusing_model);
    }

    ImGui::End();
}

void Application::UpdateModelMatrix(std::shared_ptr<PLYModel> model)
{
    if (!model)
        return;

    // Create transformation matrices
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), model->translation);
    glm::mat4 rotation_matrix_x = glm::rotate(glm::mat4(1.0f), glm::radians(model->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotation_matrix_y = glm::rotate(glm::mat4(1.0f), glm::radians(model->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotation_matrix_z = glm::rotate(glm::mat4(1.0f), glm::radians(model->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), model->scaling);

    // Combine transformations
    model->model_matrix = translation_matrix * rotation_matrix_x * rotation_matrix_y * rotation_matrix_z * scaling_matrix;

    // Update the transformed BBox
    model->transformed_bbox = model->bbox.get_transformed(model->model_matrix);
}

void Application::Initialize()
{
    // Window Preparation
    // -------------------

    if (!windowManager.CreateWindow(800, 600, "PLY Viewer"))
    {
        throw std::runtime_error("Failed to create the window for the application!");
    }
    
    windowManager.camera = &(renderer.camera);

    // GLAD Function Loader
    // -------------------

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD!");
    }

    // Renderer Preparation 
    // -------------------------------------

    // Shader Program Preparation
    renderer.Initialize();

    // Model Preparation
    const std::string defaultModelPath = "assets/ply/bunny.ply";
    if (!plyManager.LoadPLY(defaultModelPath))
    {
        throw std::runtime_error("Failed to load the default PLY model (" +  defaultModelPath + "). Exiting application.");
    }

    // Create buffer for the Model
    renderer.UploadModel(plyManager.models[0]);
    current_focusing_model = plyManager.models.back();

    // IMGUI Preparation
    // ----------------

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(windowManager.window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Frame info initialization
    // -------------------------
    last_frame = static_cast<float>(glfwGetTime());
}

void Application::Clean()
{
    if (ImGui::GetCurrentContext()) {
        ImGui::DestroyContext();
    }

    if (windowManager.window) {
        glfwDestroyWindow(windowManager.window);
        windowManager.window = nullptr;
    }

    // Terminate GLFW if it was initialized
    glfwTerminate();

    // Clean up renderer resources
    renderer.Clean();
}

void Application::Run()
{
    renderer.MakeCameraFocusOnModel(*(plyManager.models[0]));

    while (!windowManager.ShouldClose())
    {
        float current_frame = static_cast<float>(glfwGetTime());
        float delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // Update camera position
        renderer.camera.ProcessKeyboard(delta_time);

        // Clear screen
        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        // Render GUI Panels
        RenderObjectLibraryPanel();
        RenderCameraAndViewControls();
        RenderObjectManipulationPanel();

        renderer.Render(plyManager);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        windowManager.SwapBuffers();
        windowManager.PollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
