#pragma once
#include <GLFW/glfw3.h>
#include <string>

class WindowMgr
{
public:
    WindowMgr();
    ~WindowMgr();

    bool CreateWindow(int width, int height, const std::string &title);
    bool ShouldClose() const;
    void PollEvents();
    void SwapBuffers();

private:
    GLFWwindow *window;
};
