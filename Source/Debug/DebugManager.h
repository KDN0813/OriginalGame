#pragma once

#include<memory>
#include "System/ClassBase/Singleton.h"
#include "Debug/ImGuiRenderer.h"
#include "Debug/DebugPrimitiveRenderer.h"

class DebugManager : public Singleton<DebugManager>
{
public:
    DebugManager(HWND hWnd);
    ~DebugManager() {}

    ImGuiRenderer* GetImGuiRenderer() const { return this->imGui_renderer.get(); }
    DebugPrimitiveRenderer* GetDebugPrimitiveRenderer() const { return this->debug_primitive_renderer.get(); }
private:
    std::unique_ptr<ImGuiRenderer> imGui_renderer;
    std::unique_ptr<DebugPrimitiveRenderer> debug_primitive_renderer;
};

