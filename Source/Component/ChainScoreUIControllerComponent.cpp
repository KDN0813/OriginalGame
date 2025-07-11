#include "ChainScoreUIControllerComponent.h"

#include <imgui.h>

#include "Object/Object.h"
#include "TextNumberComponent.h"
#include "FadeControllerComponent.h"

void ChainScoreUIControllerComponent::ReStart()
{
}

void ChainScoreUIControllerComponent::Update(float elapsed_time)
{
}

void ChainScoreUIControllerComponent::OnScoreAdded(int value)
{
    const auto& owner = GetOwner();
    if (!owner) return;
    const auto& text_numbe = owner->GetComponent(this->text_number_Wptr);
    if (!text_numbe) return;
    text_numbe->SetDrawValue(value);
    const auto& fade_controller = owner->GetComponent(this->fade_controller_Wptr);
    if (!text_numbe) return;
    fade_controller->FeadStart(FEAD_TYPE::FEAD_IN, this->param.fead_in_time);
}

void ChainScoreUIControllerComponent::OnScoreChainEnded()
{
    const auto& owner = GetOwner();
    if (!owner) return;
    const auto& text_numbe = owner->GetComponent(this->text_number_Wptr);
    if (!text_numbe) return;
    text_numbe->SetDrawValue(0);
}

#ifdef _DEBUG

void ChainScoreUIControllerComponent::DrawDebugGUI()
{
    ImGui::InputFloat("Fead In Time", &this->param.fead_in_time);
}

#endif // _DEBUG