#pragma once
#include "resource/PLYMgr.h"
#include "viewer/Renderer.h"
#include "viewer/WindowMgr.h"

#include <string>

struct Application
{
public:
    float last_frame;
    std::shared_ptr<PLYModel> current_focusing_model;

    void Initialize();
    void Clean();
    void Run();

    void RenderObjectLibraryPanel();
    void RenderCameraAndViewControls();
    void RenderObjectManipulationPanel();
    void UpdateModelMatrix(std::shared_ptr<PLYModel> model);

private:
    WindowMgr windowManager;
    Renderer renderer;
    PLYMgr plyManager;
};