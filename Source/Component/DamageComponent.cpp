#include "DamageComponent.h"

#ifdef _DEBUG
#include "Debug\ImGuiHelper.h"
#endif // _DEBUG

#ifdef _DEBUG

void DamageComponent::DrawDebugGUI()
{
    ImGui::InputInt("DamageAmount", &this->param.damage_amount);
    ImGui::InputFloat("Invincible Time", &this->param.invincible_time);
}

#endif // _DEBUG