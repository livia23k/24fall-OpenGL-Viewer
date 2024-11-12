#include "viewer/WindowMgr.h"

#include <iostream>

WindowMgr::WindowMgr() : window(nullptr)
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
    }
}

WindowMgr::~WindowMgr()
{
    if (window)
    {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

bool WindowMgr::CreateWindow(int width, int height, const std::string &title)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    return true;
}

bool WindowMgr::ShouldClose() const
{
    return glfwWindowShouldClose(window);
}

void WindowMgr::PollEvents()
{
    glfwPollEvents();
}

void WindowMgr::SwapBuffers()
{
    glfwSwapBuffers(window);
}
