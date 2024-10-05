#pragma once

#include<memory>
#include "System/ClassBase/Singleton.h"
#include "Debug/ImGuiRenderer.h"
#include "Debug/DebugRenderer.h"

class DebugManager : public Singleton<DebugManager>
{
public:
    DebugManager(HWND hWnd, ID3D11Device* device);
    ~DebugManager() {}

    ImGuiRenderer* GetImGuiRenderer() const { return this->imGui_renderer.get(); }
    DebugRenderer* GetDebugRenderer() const { return this->debug_renderer.get(); }
private:
    std::unique_ptr<ImGuiRenderer> imGui_renderer;
    std::unique_ptr<DebugRenderer> debug_renderer;
};

