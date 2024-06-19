#include "DebugManager.h"

DebugManager::DebugManager(HWND hWnd, ID3D11Device* device)
    :Singleton(this), imGuiRenderer(std::make_unique<ImGuiRenderer>(hWnd, device))
{
}
