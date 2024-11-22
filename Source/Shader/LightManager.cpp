#include "LightManager.h"
#ifdef _DEBUG
#include "Debug\ImGuiHelper.h"
#endif // _DEBUG

#ifdef _DEBUG

void LightManager::DrawDebugGUI()
{
    if (ImGui::Begin("LightManager"))
    {
        ImGui::DragFloat4("Light Direction", &this->light_direction.x, 0.1f);
    }
    ImGui::End();
}

#endif // _DEBUG