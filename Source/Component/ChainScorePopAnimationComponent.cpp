#include "ChainScorePopAnimationComponent.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

#include <string>
#include "Object/Object.h"
#include "TextNumberComponent.h"
#include "FadeControllerComponent.h"
#include "SpriteScalerComponent.h"

void ChainScorePopAnimationComponent::ReStart()
{
}

void ChainScorePopAnimationComponent::Update(float elapsed_time)
{
}

void ChainScorePopAnimationComponent::OnScoreAdded(int value)
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

    if (this->on_score_added)
    {
        this->on_score_added(value);
    }
}

void ChainScorePopAnimationComponent::OnScoreChainStart()
{
    const auto& owner = GetOwner();
    if (!owner) return;
    const auto& fade_controller = owner->GetComponent(this->fade_controller_Wptr);
    if (!fade_controller) return;
    fade_controller->FeadStart(FEAD_TYPE::FEAD_IN, this->param.fead_in_time);
    const auto& sprite_scaler = owner->GetComponent(this->sprite_scaler_Wptr);

    if (this->on_score_chain_start)
    {
        this->on_score_chain_start();
    }
}

void ChainScorePopAnimationComponent::OnScoreChainEnd()
{
    const auto& owner = GetOwner();
    if (!owner) return;

    const auto& fade_controller = owner->GetComponent(this->fade_controller_Wptr);
    if (!fade_controller) return;
    fade_controller->FeadStart(FEAD_TYPE::FEAD_OUT, this->param.fead_out_time);
    this->param.is_chain_end_direction = true;

    if (this->on_score_chain_end)
    {
        this->on_score_chain_start();
    }
}

#ifdef _DEBUG

void ChainScorePopAnimationComponent::DrawDebugGUI()
{
    ImGui::InputFloat("Fead In Time", &this->param.fead_in_time);
    std::string text = "Is Chain End Direction:";
    text += (this->param.is_chain_end_direction ? "TRUE" : "FALSE");
    ImGui::Text(text.c_str());
    if (ImGui::Button("ScoreChainStart"))
    {
        on_score_chain_start();
    }
    if (ImGui::Button("ScoreChainEnd"))
    {
        on_score_chain_end();
    }
}

#endif // _DEBUG