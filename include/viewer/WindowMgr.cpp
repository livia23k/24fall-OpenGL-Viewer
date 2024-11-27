#include "viewer/WindowMgr.h"

#include <iostream>

WindowMgr::WindowMgr() : first_mouse(true), window_w(800), window_h(600), last_x(400), last_y(300), window(nullptr)
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

void WindowMgr::HandleFramebufferResize(int width, int height)
{
    glViewport(0, 0, width, height);

    window_w = width;
    window_h = height;

    last_x = window_w / 2.0f;
    last_y = window_h / 2.0f;
}

void WindowMgr::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    WindowMgr* mgr = static_cast<WindowMgr*>(glfwGetWindowUserPointer(window));
    if (mgr)
    {
        mgr->HandleFramebufferResize(width, height); // Call the instance-specific logic
    }
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
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, keyboard_callback);

    window_w = width;
    window_h = height;

    last_x = window_w / 2.0f;
    last_y = window_h / 2.0f;

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


void WindowMgr::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    WindowMgr* mgr = static_cast<WindowMgr*>(glfwGetWindowUserPointer(window));
    if (!mgr)
        return;

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            mgr->first_mouse = true;
        }
    }
}

void WindowMgr::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    WindowMgr* mgr = static_cast<WindowMgr*>(glfwGetWindowUserPointer(window));
    if (!mgr)
        return;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS)
        return;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (mgr->first_mouse)
    {
        mgr->last_x = xpos;
        mgr->last_y = ypos;
        mgr->first_mouse = false;
    }

    float xoffset = xpos - mgr->last_x;
    float yoffset = mgr->last_y - ypos; // reversed since y-coordinates go from bottom to top

    mgr->last_x = xpos;
    mgr->last_y = ypos;

    mgr->camera->ProcessMouseMovement(xoffset, yoffset);
}

void WindowMgr::keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    WindowMgr* mgr = static_cast<WindowMgr*>(glfwGetWindowUserPointer(window));
    if (!mgr || !mgr->camera) return;

    if (action == GLFW_PRESS) 
    {
        switch (key) {
        case GLFW_KEY_W:
            mgr->camera->movements.forward = true;
            break;
        case GLFW_KEY_S:
            mgr->camera->movements.backward = true;
            break;
        case GLFW_KEY_A:
            mgr->camera->movements.left = true;
            break;
        case GLFW_KEY_D:
            mgr->camera->movements.right = true;
            break;
        case GLFW_KEY_Q:
            mgr->camera->movements.up = true;
            break;
        case GLFW_KEY_E:
            mgr->camera->movements.down = true;
            break;
        }
    } 
    else if (action == GLFW_RELEASE) 
    {
        switch (key) {
        case GLFW_KEY_W:
            mgr->camera->movements.forward = false;
            break;
        case GLFW_KEY_S:
            mgr->camera->movements.backward = false;
            break;
        case GLFW_KEY_A:
            mgr->camera->movements.left = false;
            break;
        case GLFW_KEY_D:
            mgr->camera->movements.right = false;
            break;
        case GLFW_KEY_Q:
            mgr->camera->movements.up = false;
            break;
        case GLFW_KEY_E:
            mgr->camera->movements.down = false;
            break;
        }
    }
}
