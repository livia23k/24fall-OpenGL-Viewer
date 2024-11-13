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

void WindowMgr::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool WindowMgr::CreateWindow(int width, int height, const std::string &title)
{
    // glfw: initialize and configure
    // ------------------------------
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
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
