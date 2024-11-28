#include "viewer/Application.h"
#include "resource/PLYMgr.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>

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
    renderer.MakeCameraFocusOnModel(plyManager.models[0]);

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

        renderer.Render();

        ImGui::Begin("ImGUI Test Window");
        ImGui::Text("Hello there!");
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        windowManager.SwapBuffers();
        windowManager.PollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
