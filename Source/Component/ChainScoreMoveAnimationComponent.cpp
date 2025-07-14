#include "ChainScoreMoveAnimationComponent.h"

#include "Object/Object.h"
#include "SpriteMoverComponent.h"
#include "FadeControllerComponent.h"

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