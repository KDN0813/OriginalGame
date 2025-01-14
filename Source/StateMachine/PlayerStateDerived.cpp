#include "PlayerStateDerived.h"
#include "Input/Input.h"
#include "Object/Object.h"
#include "Object\Constant\PlayerConstant.h"
#include "Audio\Audio.h"

#include "Object\Constant\PlayerConstant.h"

#include "Component/StateMachineComponent.h"
#include "Component/ModelAnimationControlComponent.h"
#include "Component/MovementComponent.h"
#include "Component/PlayerComponent.h"
#include "Component/CircleCollisionComponent.h"
#include "Component/CharacterComponent.h"

const MyHash PlayerIdleState::STATE_NAME = MyHash("PlayerIdleState");
PlayerIdleState::PlayerIdleState()
    : State(PlayerIdleState::STATE_NAME)
{
    this->change_move_state.change_state_name = PlayerMoveState::STATE_NAME;
    this->change_attack_state.change_state_name = PlayerAttackState::STATE_NAME;
    this->change_damage_state.change_state_name = PlayerDamageState::STATE_NAME;
    this->change_spin_attack_state.change_state_name = PlayerSpinAttackState::STATE_NAME;
}

void PlayerIdleState::Staet()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    animation->PlayAnimation(PlayerConstant::ANIMATION::IDLE, true, 0.2f);
}

void PlayerIdleState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;

    auto movement = owner->GetComponent<MovementComponent>(this->movement_Wpt);
    if (!movement) return;
    if (movement->IsMoveXZAxis())
    {
        state_machine->ChangeState(this->change_move_state);
        return;
    }

    // ��_���m�F
    if (const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr); character->IsDamage())
    {
        // ��_���X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_damage_state);
        return;
    }

    // ���͎�t
    if (Input::Instance input = Input::GetInstance(); input.Get())
    {
        GamePad& pad = input->GetGamePad();
        // X�{�^��
        if (pad.GetButtonDown() & GamePad::BTN_X)
        {
            // �U���X�e�[�g�֑J��
            state_machine->ChangeState(this->change_attack_state);
        }
        // Y�{�^��
        if (pad.GetButtonDown() & GamePad::BTN_Y)
        {
            // ��]�U���X�e�[�g�ɑJ��
            state_machine->ChangeState(this->change_spin_attack_state);
            return;
        }
    }

    // TODO ���@���S�����B
    // ���S������_���[�W�X�e�[�g�ɑJ��
}

const MyHash PlayerMoveState::STATE_NAME = MyHash("PlayerMoveState");
PlayerMoveState::PlayerMoveState()
    : State(PlayerMoveState::STATE_NAME)
{
    this->change_idle_state.change_state_name = PlayerIdleState::STATE_NAME;
    this->change_damage_state.change_state_name = PlayerDamageState::STATE_NAME;
    this->change_attack_state.change_state_name = PlayerAttackState::STATE_NAME;
    this->change_spin_attack_state.change_state_name = PlayerSpinAttackState::STATE_NAME;
}

void PlayerMoveState::Staet()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    animation->PlayAnimation(PlayerConstant::ANIMATION::MOVE_FWD, true, 0.2f);
}

void PlayerMoveState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;

    auto movement = owner->GetComponent<MovementComponent>(this->movement_Wpt);
    if (!movement) return;
    if (!movement->IsMoveXZAxis())
    {
        state_machine->ChangeState(this->change_idle_state);
        return;
    }

    // ��_���m�F
    if (const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr); character->IsDamage())
    {
        // ��_���X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_damage_state);
        return;
    }

    // ���͎�t
    if (Input::Instance input = Input::GetInstance(); input.Get())
    {
        GamePad& pad = input->GetGamePad();
        // X�{�^��
        if (pad.GetButtonDown() & GamePad::BTN_X)
        {
            // �U���X�e�[�g�֑J��
            state_machine->ChangeState(this->change_attack_state);
            return;
        }
        // Y�{�^��
        if (pad.GetButtonDown() & GamePad::BTN_Y)
        {
            // ��]�U���X�e�[�g�ɑJ��
            state_machine->ChangeState(this->change_spin_attack_state);
            return;
        }
    }

    // TODO ���@���S�����B
    // ���S������_���[�W�X�e�[�g�ɑJ��
}

const MyHash PlayerAttackState::STATE_NAME = MyHash("PlayerAttackState");
PlayerAttackState::PlayerAttackState()
    : State(PlayerAttackState::STATE_NAME)
{
    this->change_idle_state.change_state_name = PlayerIdleState::STATE_NAME;
}

void PlayerAttackState::Staet()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // �A�j���[�V�����̍Đ�
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
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
    auto player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (player)
        player->SetInputMoveValidityFlag(false);

    // �U������I�u�W�F�N�g��L���ɂ���
    const auto& attack_object = owner->FindChildObject(PlayerConstant::ATTACK_OBJECT_NAME);  // �q�I�u�W�F�N�g(�U���p�I�u�W�F�N�g)�擾
    if (!attack_object) return;
    auto collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
    if (collision)
        collision->SetIsActive(true);  // �R���W������L���ɂ���

    collision->EvaluateCollision();

    // ���G��Ԃɐݒ�
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    character->SetInvincibleFlag(true);
}

void PlayerAttackState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;

    // �A�j���[�V�����Đ��҂�
    if (!animation->IsPlayAnimation())
    {
        // �ҋ@�X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_idle_state);
        return;
    }

    // TODO ���@���S�����B
    // ���S������_���[�W�X�e�[�g�ɑJ��
}

void PlayerAttackState::End()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // �v���C���[�̓��͈ړ���L���ɂ���
    auto player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (player)
        player->SetInputMoveValidityFlag(true);

    // �U������I�u�W�F�N�g�𖳌��ɂ���
    const auto& attack_object = owner->FindChildObject(PlayerConstant::ATTACK_OBJECT_NAME);  // �q�I�u�W�F�N�g(�U���p�I�u�W�F�N�g)�擾
    if (!attack_object) return;
    auto child_collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
    if (child_collision)
        child_collision->SetIsActive(false);  // �R���W�����𖳌��ɂ���

    // ���G��Ԃ�����
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    character->SetInvincibleFlag(false);
}

const MyHash PlayerSpinAttackState::STATE_NAME = MyHash("PlayerSpinAttackState");
PlayerSpinAttackState::PlayerSpinAttackState()
    :State(PlayerSpinAttackState::STATE_NAME)
{
    this->change_idle_state.change_state_name = PlayerIdleState::STATE_NAME;
}

void PlayerSpinAttackState::Staet()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // �A�j���[�V�����̍Đ�
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
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
    auto player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (player)
        player->SetInputMoveValidityFlag(false);

    // �U������I�u�W�F�N�g��L���ɂ���
    const auto& attack_object = owner->FindChildObject(PlayerConstant::SPIN_ATTACK_OBJECT_NAME);  // �q�I�u�W�F�N�g(�U���p�I�u�W�F�N�g)�擾
    if (!attack_object) return;
    auto collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
    if (collision)
        collision->SetIsActive(true);  // �R���W������L���ɂ���

    collision->EvaluateCollision();

    // ���G��Ԃɐݒ�
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    character->SetInvincibleFlag(true);
}

void PlayerSpinAttackState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;

    // �A�j���[�V�����Đ��҂�
    if (!animation->IsPlayAnimation())
    {
        // �ҋ@�X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_idle_state);
        return;
    }

    // TODO ���@���S�����B
    // ���S������_���[�W�X�e�[�g�ɑJ��
}

void PlayerSpinAttackState::End()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // �v���C���[�̓��͈ړ���L���ɂ���
    auto player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (player)
        player->SetInputMoveValidityFlag(true);

    // �U������I�u�W�F�N�g�𖳌��ɂ���
    const auto& attack_object = owner->FindChildObject(PlayerConstant::SPIN_ATTACK_OBJECT_NAME);  // �q�I�u�W�F�N�g(�U���p�I�u�W�F�N�g)�擾
    if (!attack_object) return;
    auto child_collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
    if (child_collision)
        child_collision->SetIsActive(false);  // �R���W�����𖳌��ɂ���

    // ���G��Ԃ�����
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    character->SetInvincibleFlag(false);
}


const MyHash PlayerDamageState::STATE_NAME = MyHash("PlayerDamagekState");
PlayerDamageState::PlayerDamageState()
    :State(STATE_NAME)
{
    this->change_idle_state.change_state_name = PlayerIdleState::STATE_NAME;
    this->change_move_state.change_state_name = PlayerMoveState::STATE_NAME;
    this->change_attack_state.change_state_name = PlayerAttackState::STATE_NAME;
    this->change_spin_attack_state.change_state_name = PlayerSpinAttackState::STATE_NAME;
}

void PlayerDamageState::Staet()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    animation->PlayAnimation(PlayerConstant::ANIMATION::DAMAGE, false);

    const auto& player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (!player)return;
    player->SetInputMoveValidityFlag(false);
}

void PlayerDamageState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;

    // �A�j���[�V�����Đ��҂�
    if (!animation->IsPlayAnimation())
    {
        // �ҋ@�X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_idle_state);
        return;
    }

    // TODO ���@���S�����B
    // ���S������_���[�W�X�e�[�g�ɑJ��
}

void PlayerDamageState::End()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (!player)return;
    player->SetInputMoveValidityFlag(true);
}


const MyHash PlayerDeadState::STATE_NAME = MyHash("PlayerDeadState");
PlayerDeadState::PlayerDeadState()
    :State(STATE_NAME)
{
}

void PlayerDeadState::Staet()
{
    // TODO ���@���S�����@_�@
    // �A�j���[�V�����Đ�
    // �f�X�J�����ɑJ��
}

void PlayerDeadState::Update(float elapsed_time)
{
    // TODO ���@���S�����@_�A
    // �A�j���[�V�����Đ��I���㎀�S�ҋ@�X�e�[�g�ɑJ��

    // TODO ���@���S�����C �Q�[�����[�h��ݒ�
}

// TODO ���@���S�����A
// ���S�X�e�[�g�쐬
const MyHash PlayerDeadIdleState::STATE_NAME = MyHash("PlayerDeadIdleState");
PlayerDeadIdleState::PlayerDeadIdleState()
    :State(STATE_NAME)
{
}

void PlayerDeadIdleState::Staet()
{
    // TODO ���@���S�����A
    // �A�j���[�V�����Đ�
}

void PlayerDeadIdleState::Update(float elapsed_time)
{
}
