#pragma once
#include <GLFW/glfw3.h>
#include <string>

struct WindowMgr
{
public:
    WindowMgr();
    ~WindowMgr();

    bool CreateWindow(int width, int height, const std::string &title);
    bool ShouldClose() const;
    
    void PollEvents();
    void SwapBuffers();

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

private:
    GLFWwindow *window;
};
