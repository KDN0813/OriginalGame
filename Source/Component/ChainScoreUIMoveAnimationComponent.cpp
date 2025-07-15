#include "ChainScoreUIMoveAnimationComponent.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

#include "Object/Object.h"
#include "SpriteMoverComponent.h"
#include "FadeControllerComponent.h"
#include "TextNumberComponent.h"

void ChainScoreMoveAnimationComponent::OnScoreAdded(int value)
{
    const auto& owner = GetOwner();
    if (!owner) return;
    const auto& text_numbe = owner->GetComponent(this->text_number_Wptr);
    if (!text_numbe) return;
    text_numbe->SetDrawValue(value);
}

void ChainScoreMoveAnimationComponent::OnScoreChainEnd()
{
    const auto& owner = GetOwner();
    if (!owner) return;

    const auto& fade_controller = owner->GetComponent(this->fade_controller_Wptr);
    if (!fade_controller) return;
    fade_controller->SetAlpha(1.0f);
    fade_controller->FeadStart(FEAD_TYPE::FEAD_OUT, this->param.fead_out_time);

    const auto& sprite_mover = owner->GetComponent(this->sprite_mover_Wptr);
    if (!sprite_mover) return;
    sprite_mover->CommandClear();
    sprite_mover->PushBackCommand(this->param.initial_pos, 0.0f);
    sprite_mover->PushBackCommand(this->param.target_pos, this->param.fead_out_time);
    sprite_mover->PushBackCommand(this->param.initial_pos, 0.0f);
}

#ifdef _DEBUG

void ChainScoreMoveAnimationComponent::DrawDebugGUI()
{
    ImGui::InputFloat("Fead Out Time", &this->param.fead_out_time);
    ImGui::InputFloat2("Initial Pos", &this->param.initial_pos.x);
    ImGui::InputFloat2("Target Pos", &this->param.target_pos.x);

    if (ImGui::Button("OnScoreChainEnd"))
    {
        OnScoreChainEnd();
    }
}

#endif // _DEBUG