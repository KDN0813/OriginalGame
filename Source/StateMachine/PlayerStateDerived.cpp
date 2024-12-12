#include "PlayerStateDerived.h"
#include "Input/Input.h"
#include "Object/Object.h"
#include "Object\Constant\PlayerConstant.h"
#include "Audio\Audio.h"

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
    this->change_spin_attack_state.change_state_name = MyHash("SpinAttackState");
}

void PlayerIdleState::Staet()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    auto animation = owner->EnsureComponentValid<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    animation->PlayAnimation(PlayerConstant::ANIMATION::IDLE, true, 0.2f);
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
        return;
    }

    // ���͎�t
    if (Input::Instance input = Input::GetInstance(); input.Get())
    {
        GamePad& pad = input->GetGamePad();
        if (pad.GetButtonDown() & GamePad::BTN_X)
        {
            state_machine->ChangeState(this->change_attack_state);
            return;
        }
        if (pad.GetButtonDown() & GamePad::BTN_Y)
        {
            state_machine->ChangeState(this->change_spin_attack_state);
            return;
        }
    }
}

PlayerMoveState::PlayerMoveState()
    : State("PlayerMoveState")
{
    this->change_idle_state.change_state_name = MyHash("PlayerIdleState");
    this->change_attack_state.change_state_name = MyHash("PlayerAttackState");
    this->change_spin_attack_state.change_state_name = MyHash("SpinAttackState");
}

void PlayerMoveState::Staet()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    auto animation = owner->EnsureComponentValid<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    animation->PlayAnimation(PlayerConstant::ANIMATION::MOVE_FWD, true, 0.2f);
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
        return;
    }

    // ���͎�t
    if (Input::Instance input = Input::GetInstance(); input.Get())
    {
        GamePad& pad = input->GetGamePad();
        if (pad.GetButtonDown() & GamePad::BTN_X)
        {
            state_machine->ChangeState(this->change_attack_state);
            return;
        }
        if (pad.GetButtonDown() & GamePad::BTN_Y)
        {
            state_machine->ChangeState(this->change_spin_attack_state);
            return;
        }
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

    // �A�j���[�V�����̍Đ�
    auto animation = owner->EnsureComponentValid<ModelAnimationControlComponent>(this->animation_Wprt);
    if (animation)
        animation->PlayAnimation(PlayerConstant::ANIMATION::ATTACK01, false, 0.2f);

    // SE�Đ�
    if (Audio::Instance audio = Audio::GetInstance(); audio.Get())
    {
        AudioParam param{};
        param.volume = 0.5f;
        param.loop = false;
        param.filename = "Data/Audio/SE_Slash01.wav";
        audio->Play(param);
    }

    // �v���C���[�̓��͈ړ��𖳌��ɂ���
    auto player = owner->EnsureComponentValid<PlayerComponent>(this->player_Wprt);
    if (player)
        player->SetInputMoveValidityFlag(false);

    // �U������I�u�W�F�N�g��L���ɂ���
    const auto& attack_object = owner->FindChildObject(MyHash("AttackObject"));  // �q�I�u�W�F�N�g(�U���p�I�u�W�F�N�g)�擾
    if (!attack_object) return;
    auto collision = attack_object->EnsureComponentValid<CircleCollisionComponent>(this->child_collision_Wprt);
    if (collision)
        collision->SetIsActive(true);  // �R���W������L���ɂ���

    collision->EvaluateCollision();
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
        return;
    }
}

void PlayerAttackState::End()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // �v���C���[�̓��͈ړ���L���ɂ���
    auto player = owner->EnsureComponentValid<PlayerComponent>(this->player_Wprt);
    if (player)
        player->SetInputMoveValidityFlag(true);

    // �U������I�u�W�F�N�g�𖳌��ɂ���
    const auto& attack_object = owner->FindChildObject(MyHash("AttackObject"));  // �q�I�u�W�F�N�g(�U���p�I�u�W�F�N�g)�擾
    if (!attack_object) return;
    auto child_collision = attack_object->EnsureComponentValid<CircleCollisionComponent>(this->child_collision_Wprt);
    if (child_collision)
        child_collision->SetIsActive(false);  // �R���W�����𖳌��ɂ���
}

PlayerSpinAttackState::PlayerSpinAttackState()
    :State("SpinAttackState")
{
    this->change_idle_state.change_state_name = MyHash("PlayerIdleState");
}

void PlayerSpinAttackState::Staet()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // �A�j���[�V�����̍Đ�
    auto animation = owner->EnsureComponentValid<ModelAnimationControlComponent>(this->animation_Wprt);
    if (animation)
        animation->PlayAnimation(PlayerConstant::ANIMATION::SPIN_ATTACK, false, 0.2f);

    // SE�Đ�
    if (Audio::Instance audio = Audio::GetInstance(); audio.Get())
    {
        AudioParam param{};
        param.volume = 0.4f;
        param.loop = false;
        param.filename = "Data/Audio/SE_Slash02.wav";
        audio->Play(param);
    }

    // �v���C���[�̓��͈ړ��𖳌��ɂ���
    auto player = owner->EnsureComponentValid<PlayerComponent>(this->player_Wprt);
    if (player)
        player->SetInputMoveValidityFlag(false);

    // �U������I�u�W�F�N�g��L���ɂ���
    const auto& attack_object = owner->FindChildObject(MyHash("SpinAttackObject"));  // �q�I�u�W�F�N�g(�U���p�I�u�W�F�N�g)�擾
    if (!attack_object) return;
    auto collision = attack_object->EnsureComponentValid<CircleCollisionComponent>(this->child_collision_Wprt);
    if (collision)
        collision->SetIsActive(true);  // �R���W������L���ɂ���

    collision->EvaluateCollision();
}

void PlayerSpinAttackState::Update(float elapsed_time)
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
        return;
    }
}

void PlayerSpinAttackState::End()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // �v���C���[�̓��͈ړ���L���ɂ���
    auto player = owner->EnsureComponentValid<PlayerComponent>(this->player_Wprt);
    if (player)
        player->SetInputMoveValidityFlag(true);

    // �U������I�u�W�F�N�g�𖳌��ɂ���
    const auto& attack_object = owner->FindChildObject(MyHash("SpinAttackObject"));  // �q�I�u�W�F�N�g(�U���p�I�u�W�F�N�g)�擾
    if (!attack_object) return;
    auto child_collision = attack_object->EnsureComponentValid<CircleCollisionComponent>(this->child_collision_Wprt);
    if (child_collision)
        child_collision->SetIsActive(false);  // �R���W�����𖳌��ɂ���
}
