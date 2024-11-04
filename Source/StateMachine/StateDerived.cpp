#include "StateDerived.h"
#include "Object/Object.h"
#include "ConstantManager.h"

#include "Component/StateMachineComponent.h"
#include "Component/ModelAnimationControlComponent.h"
#include "Component/MovementComponent.h"

PlayerIdleState::PlayerIdleState()
    : State("PlayerIdleState")
{
    change_move_state.change_state_name = MyHash("PlayerMoveState");
}

void PlayerIdleState::Staet()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    auto animation = owner->EnsureComponentValid<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    animation->PlayAnimation(PlayerCT::ANIMATION::IDLE, true, 0.2f);
}

void PlayerIdleState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->EnsureComponentValid<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;

    auto movement = owner->EnsureComponentValid<MovementComponent>(this->movement_Wpt);
    if (!movement) return;
    if (movement->IsMoveXZAxis())
    {
        state_machine->ChangeState(this->change_move_state);
    }
}

PlayerMoveState::PlayerMoveState()
    : State("PlayerMoveState")
{
    change_idle_state.change_state_name = MyHash("PlayerIdleState");
}

void PlayerMoveState::Staet()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    auto animation = owner->EnsureComponentValid<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    animation->PlayAnimation(PlayerCT::ANIMATION::MOVE_FWD, true, 0.2f);
}

void PlayerMoveState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->EnsureComponentValid<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;

    auto movement = owner->EnsureComponentValid<MovementComponent>(this->movement_Wpt);
    if (!movement) return;
    if (!movement->IsMoveXZAxis())
    {
        state_machine->ChangeState(this->change_idle_state);
    }
}
