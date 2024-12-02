#include "viewer/Application.h"
#include "resource/PLYMgr.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "tinyfd/tinyfiledialogs.h"

#include <glad/glad.h>
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

            current_focusing_model = plyManager.models.back();
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
        // TODO: Set camera to top view
    }
    if (ImGui::Button("Front View"))
    {
        // TODO: Set camera to front view
    }
    if (ImGui::Button("Side View"))
    {
        // TODO: Set camera to side view
    }

    ImGui::End();
}

void Application::RenderObjectManipulationPanel()
{
    ImGui::Begin("Object Manipulation & Properties");

    ImGui::SeparatorText("Item Details");
    // ImGui::Text("Name: %s", currentModel.name.c_str());
    // ImGui::Text("Manufacturer: %s", currentModel.manufacturer.c_str());
    // ImGui::Text("Price: %.2f", currentModel.price);
    // ImGui::Text("Size: %s", currentModel.size.c_str());

    ImGui::SeparatorText("Transform");
    // ImGui::SliderFloat3("Translation", currentModel.translation, -10.0f, 10.0f);
    // ImGui::SliderFloat3("Rotation", currentModel.rotation, -180.0f, 180.0f);
    // ImGui::SliderFloat3("Scaling", currentModel.scaling, 0.1f, 10.0f);
    if (ImGui::Button("Apply Transform"))
    {
        // TODO: Apply transformation to the selected model
    }

    ImGui::SeparatorText("Appearance");
    if (ImGui::Button("Toggle Color/Texture"))
    {
        // TODO: Switch between color and texture
    }

    ImGui::SeparatorText("Vertices Properties");
    // if (ImGui::Checkbox("Show Normals", &currentModel.showNormals))
    // {
    //     // TODO: Toggle normal visualization
    // }
    // if (ImGui::Checkbox("Show Tangents", &currentModel.showTangents))
    // {
    //     // TODO: Toggle tangent visualization
    // }

    ImGui::End();
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
