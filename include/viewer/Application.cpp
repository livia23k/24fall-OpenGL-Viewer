#include "viewer/Application.h"
#include "resource/PLYMgr.h"

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
        std::cerr << "Failed to initialize the application!" << std::endl;
        return;
    }

    // GLAD Function Loader
    // -------------------

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return;
    }

    // Renderer Preparation 
    // -------------------------------------

    // Shader Program Preparation
    renderer.Initialize();

    // Model Preparation
    if (!plyManager.LoadPLY("assets/ply/cube.ply"))
    {
        std::cerr << "Failed to load the PLY model!" << std::endl;
        return;
    }

    // Create buffer for the Model
    renderer.UploadModel(plyManager.models[0]);
}

void Application::Run()
{
    renderer.MakeCameraFocusOnModel(plyManager.models[0]);

    while (!windowManager.ShouldClose())
    {
        // Clear screen
        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.Render();

        windowManager.SwapBuffers();
        windowManager.PollEvents();
    }
}
