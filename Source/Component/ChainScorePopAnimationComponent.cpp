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
    sprite_scaler->PushBackCommand(this->param.expanded.target_scale,this->param.expanded.time);// スプライトの拡大指示
    sprite_scaler->PushBackCommand(this->param.shrink.target_scale, this->param.shrink.time);// スプライトの縮小指示

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
    fade_controller->SetAlpha(0.0f);

    if (this->on_score_chain_end)
    {
        this->on_score_chain_end();
    }
}

#ifdef _DEBUG

void ChainScorePopAnimationComponent::DrawDebugGUI()
{
    ImGui::InputFloat("Fead In Time", &this->param.fead_in_time);
    
    ImGui::InputFloat2("Expanded Scale", &this->param.expanded.target_scale.x);
    ImGui::InputFloat("Expanded Time", &this->param.expanded.time);

    ImGui::InputFloat2("Shrink Scale", &this->param.shrink.target_scale.x);
    ImGui::InputFloat("Shrink Time", &this->param.shrink.time);

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