#include "StateDerived.h"
#include "Input/Input.h"
#include "Object/Object.h"
#include "ConstantManager.h"

#include "Component/StateMachineComponent.h"
#include "Component/ModelAnimationControlComponent.h"
#include "Component/MovementComponent.h"
#include "Component/PlayerComponent.h"
#include "Component/CircleCollisionComponent.h"

PlayerIdleState::PlayerIdleState()
    : State("PlayerIdleState")
{
    this->change_move_state.change_state_name = MyHash("PlayerMoveState");
    this->change_attack_state.change_state_name = MyHash("PlayerAttackState");
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

    GamePad& pad = Input::Instance()->GetGamePad();
    if (pad.GetButtonDown() & GamePad::BTN_X)
    {
        state_machine->ChangeState(this->change_attack_state);
    }
}

PlayerMoveState::PlayerMoveState()
    : State("PlayerMoveState")
{
    this->change_idle_state.change_state_name = MyHash("PlayerIdleState");
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

PlayerAttackState::PlayerAttackState()
    : State("PlayerAttackState")
{
    this->change_idle_state.change_state_name = MyHash("PlayerIdleState");
}

void PlayerAttackState::Staet()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    auto animation = owner->EnsureComponentValid<ModelAnimationControlComponent>(this->animation_Wprt);
    if (animation)
        animation->PlayAnimation(PlayerCT::ANIMATION::ATTACK01, false, 0.2f);

    auto player = owner->EnsureComponentValid<PlayerComponent>(this->player_Wprt);
    if (player)
        player->SetIsActive(false);  // コントロールを無効にする

    auto collision = owner->EnsureComponentValid<CircleCollisionComponent>(this->collision_Wprt);
    if (collision)
        collision->SetIsActive(true);  // コリジョンを有効にする
}

void PlayerAttackState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->EnsureComponentValid<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    auto animation = owner->EnsureComponentValid<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;

    if (!animation->IsPlayAnimation())
    {
        state_machine->ChangeState(this->change_idle_state);
    }
    return;
}

void PlayerAttackState::End()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    auto player = owner->EnsureComponentValid<PlayerComponent>(this->player_Wprt);
    if (player)
        player->SetIsActive(true);  // コントロールを有効にする

    auto collision = owner->EnsureComponentValid<CircleCollisionComponent>(this->collision_Wprt);
    if (collision)
        collision->SetIsActive(false);  // コリジョンを無効にする
}
