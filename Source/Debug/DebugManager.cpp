#include "DebugManager.h"

DebugManager::DebugManager(HWND hWnd, ID3D11Device* device)
    :Singleton(this),
    imGui_renderer(std::make_unique<ImGuiRenderer>(hWnd, device)),
    debug_renderer(std::make_unique<DebugRenderer>(device))
{
}
