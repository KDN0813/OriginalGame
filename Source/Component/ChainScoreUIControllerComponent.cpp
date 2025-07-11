#include "ChainScoreUIControllerComponent.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

#include <string>
#include "Object/Object.h"
#include "TextNumberComponent.h"
#include "FadeControllerComponent.h"

void ChainScoreUIControllerComponent::ReStart()
{
}

void ChainScoreUIControllerComponent::Update(float elapsed_time)
{
    if (this->param.is_chain_end_direction)
    {

    }
}

void ChainScoreUIControllerComponent::OnScoreAdded(int value)
{
    const auto& owner = GetOwner();
    if (!owner) return;
    const auto& text_numbe = owner->GetComponent(this->text_number_Wptr);
    if (!text_numbe) return;
    text_numbe->SetDrawValue(value);
}

void ChainScoreUIControllerComponent::OnScoreChainStart()
{
    const auto& owner = GetOwner();
    if (!owner) return;
    const auto& fade_controller = owner->GetComponent(this->fade_controller_Wptr);
    if (!fade_controller) return;
    fade_controller->FeadStart(FEAD_TYPE::FEAD_IN, this->param.fead_in_time);
}

void ChainScoreUIControllerComponent::OnScoreChainEnd()
{
    const auto& owner = GetOwner();
    if (!owner) return;
    const auto& fade_controller = owner->GetComponent(this->fade_controller_Wptr);
    if (!fade_controller) return;
    fade_controller->FeadStart(FEAD_TYPE::FEAD_OUT, this->param.fead_in_time);
    this->param.is_chain_end_direction = true;
}

#ifdef _DEBUG

void ChainScoreUIControllerComponent::DrawDebugGUI()
{
    ImGui::InputFloat("Fead In Time", &this->param.fead_in_time);
    std::string text = "Is Chain End Direction:";
    text += (this->param.is_chain_end_direction ? "TRUE" : "FALSE");
    ImGui::Text(text.c_str());
    if (ImGui::Button("ScoreChainStart"))
    {
        OnScoreChainStart();
    }
    if (ImGui::Button("ScoreChainEnd"))
    {
        OnScoreChainEnd();
    }
}

#endif // _DEBUG