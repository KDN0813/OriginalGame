#include "DebugManager.h"
#include "Graphics/Graphics.h"

DebugManager::DebugManager(HWND hWnd)
    :Singleton(this)
{
    Graphics* graphics = Graphics::Instance();
    std::lock_guard<std::mutex> lock(graphics->GetInstanceMutex());
    ID3D11Device*  device = graphics->GetDevice();

    this->imGui_renderer = std::make_unique<ImGuiRenderer>(hWnd, device);
    this->debug_primitive_renderer = std::make_unique<DebugPrimitiveRenderer>(device);
}
