#include "ChainScoreUIControllerComponent.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

#include <string>
#include "Object/Object.h"
#include "TextNumberComponent.h"
#include "FadeControllerComponent.h"
#include "SpriteScalerComponent.h"
#include "SpriteMoverComponent.h"

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


    const auto& sprite_scaler = owner->GetComponent(this->sprite_scaler_Wptr);
    if (!sprite_scaler) return;
    sprite_scaler->CommandClear();
    sprite_scaler->PushBackCommand(this->param.expanded_scale,this->param.time_to_expand);// スプライトの拡大指示
    sprite_scaler->PushBackCommand(this->param.shrink_scale, this->param.time_to_shrink);// スプライトの縮小指示

}

void ChainScoreUIControllerComponent::OnScoreChainStart()
{
    const auto& owner = GetOwner();
    if (!owner) return;
    const auto& fade_controller = owner->GetComponent(this->fade_controller_Wptr);
    if (!fade_controller) return;
    fade_controller->FeadStart(FEAD_TYPE::FEAD_IN, this->param.fead_in_time);
    const auto& sprite_scaler = owner->GetComponent(this->sprite_scaler_Wptr);
}

void ChainScoreUIControllerComponent::OnScoreChainEnd()
{
    const auto& owner = GetOwner();
    if (!owner) return;

    const auto& fade_controller = owner->GetComponent(this->fade_controller_Wptr);
    if (!fade_controller) return;
    fade_controller->FeadStart(FEAD_TYPE::FEAD_OUT, this->param.fead_in_time);
    this->param.is_chain_end_direction = true;

    const auto& sprite_mover = owner->GetComponent(this->sprite_mover_Wptr);
    if (!sprite_mover) return;
    sprite_mover->PushFrontCommand(this->param.target_pos, this->param.fead_out_time);
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