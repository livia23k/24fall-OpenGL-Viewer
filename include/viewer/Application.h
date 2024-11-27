#pragma once
#include "resource/PLYMgr.h"
#include "viewer/Renderer.h"
#include "viewer/WindowMgr.h"

#include <string>

struct Application
{
public:
    float last_frame;

    void Initialize();
    void Run();

private:
    WindowMgr windowManager;
    Renderer renderer;
    PLYMgr plyManager;
};