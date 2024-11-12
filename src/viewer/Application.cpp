#include "viewer/Application.h"

#include <iostream>

void Application::Run()
{
    if (!windowManager.CreateWindow(800, 600, "PLY Viewer"))
    {
        std::cerr << "Failed to initialize the application!" << std::endl;
        return;
    }

    while (!windowManager.ShouldClose())
    {
        windowManager.PollEvents();

        // Clear screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        windowManager.SwapBuffers();
    }
}
