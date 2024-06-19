#pragma once

#include<memory>
#include "System/ClassBase/Singleton.h"
#include "Debug/ImGuiRenderer.h"

class DebugManager : public Singleton<DebugManager>
{
public:
    DebugManager(HWND hWnd, ID3D11Device* device);
    ~DebugManager() {}

    ImGuiRenderer* GetImGuiRenderer() const { return this->imGuiRenderer.get(); }
private:
    std::unique_ptr<ImGuiRenderer> imGuiRenderer;
};

