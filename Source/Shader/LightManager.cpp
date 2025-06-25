#include "LightManager.h"
#ifdef _DEBUG
#include "Debug\ImGuiHelper.h"
#endif // _DEBUG

#ifdef _DEBUG

void LightManager::DrawDebugGUI()
{
    if (ImGui::Begin("LightManager"))
    {
        ImGui::ColorEdit4("Ambient Color", &this->ambient_color.x);
        if (ImGui::CollapsingHeader("Direction Light"))
        {
            ImGui::DragFloat4("Direction##DirectionLight", &this->direction_light.direction.x, 0.1f);
            ImGui::ColorEdit4("Color##DirectionLight", &this->direction_light.color.x);
        }
    }
    ImGui::End();
}

#endif // _DEBUG
