#pragma once
#include "camera/Camera.h"

#include <GLFW/glfw3.h>
#include <string>

struct WindowMgr
{
public:
    Camera *camera;
    
    WindowMgr();
    ~WindowMgr();

    bool CreateWindow(int width, int height, const std::string &title);
    bool ShouldClose() const;
    
    void PollEvents();
    void SwapBuffers();

    void HandleFramebufferResize(int width, int height);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

private:
    bool first_mouse;

    float window_w;
    float window_h;
    float last_x;
    float last_y;
    
    GLFWwindow *window;
};
