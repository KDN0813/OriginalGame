#include "PlayerStateDerived.h"
#include "Input/Input.h"
#include "Object/Object.h"
#include "Object\Constant\PlayerConstant.h"
#include "Audio\Audio.h"
#include "System\GameData.h"

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
    this->change_spin_attack_state.change_state_name = PlayerSpinAttackSpinLoopState::STATE_NAME;
    this->change_dead_state.change_state_name = PlayerDeadState::STATE_NAME;
}

void PlayerIdleState::Start()
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
    const auto& player = owner->GetComponent<PlayerComponent>(this->player_Wptr);
    if (!player) return;

    auto movement = owner->GetComponent<MovementComponent>(this->movement_Wpt);
    if (!movement) return;
    if (movement->IsMoveXZAxis())
    {
        state_machine->ChangeState(this->change_move_state);
        return;
    }

    if (const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr); character.get())
    {
        // ���S����
        if (!character->IsAlive())
        {
            // ��_���X�e�[�g�ɑJ��
            state_machine->ChangeState(this->change_dead_state);
            return;
        }

        // ��_������
#if 0
        if (character->IsDamage())
        {
            // ��_���X�e�[�g�ɑJ��
            state_machine->ChangeState(this->change_damage_state);
            return;
        }
#endif // 0
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
            if (player->IsUseSpecialGage(player->GetSpinAttackUseGageCount()))
            {
                state_machine->ChangeState(this->change_spin_attack_state);
                return;
            }
        }
    }
}

const MyHash PlayerMoveState::STATE_NAME = MyHash("PlayerMoveState");
PlayerMoveState::PlayerMoveState()
    : State(PlayerMoveState::STATE_NAME)
{
    this->change_idle_state.change_state_name = PlayerIdleState::STATE_NAME;
    this->change_damage_state.change_state_name = PlayerDamageState::STATE_NAME;
    this->change_attack_state.change_state_name = PlayerAttackState::STATE_NAME;
    this->change_spin_attack_state.change_state_name = PlayerSpinAttackSpinLoopState::STATE_NAME;
    this->change_dead_state.change_state_name = PlayerDeadState::STATE_NAME;
}

void PlayerMoveState::Start()
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
    const auto& player = owner->GetComponent<PlayerComponent>(this->player_Wptr);
    if (!player) return;

    auto movement = owner->GetComponent<MovementComponent>(this->movement_Wpt);
    if (!movement) return;
    if (!movement->IsMoveXZAxis())
    {
        state_machine->ChangeState(this->change_idle_state);
        return;
    }

    if (const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr); character.get())
    {
        // ���S����
        if (!character->IsAlive())
        {
            // ��_���X�e�[�g�ɑJ��
            state_machine->ChangeState(this->change_dead_state);
            return;
        }

        // ��_������
#if 0
        if (character->IsDamage())
        {
            // ��_���X�e�[�g�ɑJ��
            state_machine->ChangeState(this->change_damage_state);
            return;
        }
#endif // 0
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
            if (player->IsUseSpecialGage(player->GetSpinAttackUseGageCount()))
            {
                state_machine->ChangeState(this->change_spin_attack_state);
                return;
            }
        }
    }
}

const MyHash PlayerAttackState::STATE_NAME = MyHash("PlayerAttackState");
PlayerAttackState::PlayerAttackState()
    : State(PlayerAttackState::STATE_NAME)
{
    this->change_dead_state.change_state_name = PlayerDeadState::STATE_NAME;
    this->change_attack_combo2_state.change_state_name = PlayerAttackLCombo2State::STATE_NAME;
    this->change_attack_hold_state.change_state_name = PlayerAttackHoldState::STATE_NAME;
}

void PlayerAttackState::Start()
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
    const auto& attack_object = owner->FindChildObject(PlayerConstant::ATTACK01_OBJECT_NAME);  // �q�I�u�W�F�N�g(�U���p�I�u�W�F�N�g)�擾
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
    const auto& animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    const auto& player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (!player) return;

    if (const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr); character.get())
    {
        // ���S����
        if (!character->IsAlive())
        {
            // ��_���X�e�[�g�ɑJ��
            state_machine->ChangeState(this->change_dead_state);
            return;
        }
    }

    // ���͎�t
    if (Input::Instance input = Input::GetInstance(); input.Get())
    {
        GamePad& pad = input->GetGamePad();
        // X�{�^��
        if (pad.GetButtonDown() & GamePad::BTN_X)
        {
            // �U���X�e�[�g�֑J��
            state_machine->ChangeState(this->change_attack_combo2_state);
            return;
        }
    }

    // �A�j���[�V�����Đ��҂�
    if (player->attack_end_point <= animation->GetCurrentAnimationSeconds())
    {
        // �ҋ@�X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_attack_hold_state);
        return;
    }
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
    const auto& attack_object = owner->FindChildObject(PlayerConstant::ATTACK01_OBJECT_NAME);  // �q�I�u�W�F�N�g(�U���p�I�u�W�F�N�g)�擾
    if (!attack_object) return;
    auto child_collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
    if (child_collision)
        child_collision->SetIsActive(false);  // �R���W�����𖳌��ɂ���

    // ���G��Ԃ�����
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    character->SetInvincibleFlag(false);
}

const MyHash PlayerAttackLCombo2State::STATE_NAME = MyHash("PlayerAttackLCombo2State");
PlayerAttackLCombo2State::PlayerAttackLCombo2State()
    : State(PlayerAttackLCombo2State::STATE_NAME)
{
    this->change_dead_state.change_state_name = PlayerDeadState::STATE_NAME;
    this->change_attack_state.change_state_name = PlayerAttackState::STATE_NAME;
    this->change_attack_hold_state.change_state_name = PlayerAttackHoldState::STATE_NAME;
}

void PlayerAttackLCombo2State::Start()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // �A�j���[�V�����̍Đ�
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (animation)
        animation->PlayAnimation(PlayerConstant::ANIMATION::ATTACK02, false, 0.2f);

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
    const auto& attack_object = owner->FindChildObject(PlayerConstant::ATTACK02_OBJECT_NAME);  // �q�I�u�W�F�N�g(�U���p�I�u�W�F�N�g)�擾
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

void PlayerAttackLCombo2State::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    const auto& player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (!player) return;

    if (const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr); character.get())
    {
        // ���S����
        if (!character->IsAlive())
        {
            // ��_���X�e�[�g�ɑJ��
            state_machine->ChangeState(this->change_dead_state);
            return;
        }
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
    }

    // �A�j���[�V�����Đ��҂�
    if (player->attack_combo2_end_point <= animation->GetCurrentAnimationSeconds())
    {
        // �ҋ@�X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_attack_hold_state);
        return;
    }
}

void PlayerAttackLCombo2State::End()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // �v���C���[�̓��͈ړ���L���ɂ���
    auto player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (player)
        player->SetInputMoveValidityFlag(true);

    // �U������I�u�W�F�N�g�𖳌��ɂ���
    const auto& attack_object = owner->FindChildObject(PlayerConstant::ATTACK02_OBJECT_NAME);  // �q�I�u�W�F�N�g(�U���p�I�u�W�F�N�g)�擾
    if (!attack_object) return;
    auto child_collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
    if (child_collision)
        child_collision->SetIsActive(false);  // �R���W�����𖳌��ɂ���

    // ���G��Ԃ�����
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    character->SetInvincibleFlag(false);
}

const MyHash PlayerAttackHoldState::STATE_NAME = MyHash("PlayerAttackHoldState");
PlayerAttackHoldState::PlayerAttackHoldState()
    :State(STATE_NAME)
{
    this->change_idle_state.change_state_name = PlayerIdleState::STATE_NAME;
    this->change_move_state.change_state_name = PlayerMoveState::STATE_NAME;
    this->change_dead_state.change_state_name = PlayerDeadState::STATE_NAME;
    this->change_spin_attack_state.change_state_name = PlayerSpinAttackSpinLoopState::STATE_NAME;
}

void PlayerAttackHoldState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    const auto& player = owner->GetComponent<PlayerComponent>(this->player_Wptr);
    if (!player) return;

    if (const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr); character.get())
    {
        // ���S����
        if (!character->IsAlive())
        {
            // ��_���X�e�[�g�ɑJ��
            state_machine->ChangeState(this->change_dead_state);
            return;
        }
    }

    // ���͎�t
    if (Input::Instance input = Input::GetInstance(); input.Get())
    {
        GamePad& pad = input->GetGamePad();
        // Y�{�^��
        if (pad.GetButton() & GamePad::BTN_Y)
        {
            // ��]�U���X�e�[�g�ɑJ��
            if (player->IsUseSpecialGage(player->GetSpinAttackUseGageCount()))
            {
                state_machine->ChangeState(this->change_spin_attack_state);
                return;
            }
        }
    }

    // �ړ�����
    auto movement = owner->GetComponent<MovementComponent>(this->movement_Wpt);
    if (!movement) return;
    if (movement->IsMoveXZAxis())
    {
        state_machine->ChangeState(this->change_move_state);
        return;
    }

    // �A�j���[�V�����Đ��҂�
    if (!animation->IsPlayAnimation())
    {
        // �ҋ@�X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_idle_state);
        return;
    }
}

const MyHash PlayerSpinAttackStartState::STATE_NAME = MyHash("PlayerSpinAttackStartState");
PlayerSpinAttackStartState::PlayerSpinAttackStartState()
    :State(STATE_NAME)
{
    this->change_spin_attack_state.change_state_name = PlayerSpinAttackSpinLoopState::STATE_NAME;
}

void PlayerSpinAttackStartState::Start()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    // �A�j���[�V�����̍Đ�
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (animation)
        animation->PlayAnimation(PlayerConstant::ANIMATION::SPIN_ATTACK_START, false, 0.2f);
}

void PlayerSpinAttackStartState::Update(float elapsed_time)
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
        // ��]
        state_machine->ChangeState(this->change_spin_attack_state);
        return;
    }
}


const MyHash PlayerSpinAttackSpinLoopState::STATE_NAME = MyHash("PlayerSpinAttackSpinLoopState");
PlayerSpinAttackSpinLoopState::PlayerSpinAttackSpinLoopState()
    :State(STATE_NAME)
{
    this->change_idle_state.change_state_name = PlayerIdleState::STATE_NAME;
    this->change_dead_state.change_state_name = PlayerDeadState::STATE_NAME;
}

void PlayerSpinAttackSpinLoopState::Start()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // �A�j���[�V�����̍Đ�
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (animation)
        animation->PlayAnimation(PlayerConstant::ANIMATION::SPIN_ATTACK_SPIN_LOOP, true, 0.0f);

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
    {
        player->SetIsSpinAttack(true);  // ��]�U���t���O�𗧂Ă�
        player->SetMoveRate(player->GetSpinAttackMoveRate());
        this->attack_time = player->GetSpinAttackTime();    // �U�����Ԃ�ݒ�
    }

    // �U������I�u�W�F�N�g��L���ɂ���
    const auto& attack_object = owner->FindChildObject(PlayerConstant::SPIN_ATTACK_OBJECT_NAME);  // �q�I�u�W�F�N�g(�U���p�I�u�W�F�N�g)�擾
    if (!attack_object) return;
    auto collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
    if (!collision) return;

    collision->SetIsActive(true);  // �R���W������L���ɂ���
    collision->EvaluateCollision();

    // ���G��Ԃɐݒ�
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    character->SetInvincibleFlag(true);

    // �|�C���g����
    {
        player->UseSpecialGage(player->GetSpinAttackUseGageCount());
    }
}

void PlayerSpinAttackSpinLoopState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    const auto& player = owner->GetComponent<PlayerComponent>(this->player_Wptr);
    if (!player) return;

    if (const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr); character.get())
    {
        // ���S����
        if (!character->IsAlive())
        {
            // ��_���X�e�[�g�ɑJ��
            state_machine->ChangeState(this->change_dead_state);
            return;
        }
    }

    // �U������I�u�W�F�N�g��L���ɂ���
    {
        const auto& attack_object = owner->FindChildObject(PlayerConstant::SPIN_ATTACK_OBJECT_NAME);  // �q�I�u�W�F�N�g(�U���p�I�u�W�F�N�g)�擾
        if (!attack_object) return;
        auto collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
        if (!collision) return;
        collision->EvaluateCollision();
    }

    this->attack_time -= elapsed_time;
    if (this->attack_time <= 0.0f)
    {
        // �U�����Ԃ�0�ɂȂ�����ҋ@�X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_idle_state);
    }
}

void PlayerSpinAttackSpinLoopState::End()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // �v���C���[�̓��͈ړ���L���ɂ���
    auto player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (player)
    {
        player->SetIsSpinAttack(false);  // ��]�U���t���O��܂�
        player->SetInputMoveValidityFlag(true);
        player->SetMoveRate(1.0f);
    }

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

const MyHash PlayerSpinAttackState::STATE_NAME = MyHash("PlayerSpinAttackState");
PlayerSpinAttackState::PlayerSpinAttackState()
    :State(PlayerSpinAttackState::STATE_NAME)
{
    this->change_idle_state.change_state_name = PlayerIdleState::STATE_NAME;
    this->change_dead_state.change_state_name = PlayerDeadState::STATE_NAME;
}

void PlayerSpinAttackState::Start()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // �A�j���[�V�����̍Đ�
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (animation)
        animation->PlayAnimation(PlayerConstant::ANIMATION::SPIN_ATTACK, false, 0.0f);

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
    {
        //player->SetInputMoveValidityFlag(false);
        player->SetMoveRate(player->GetSpinAttackMoveRate());
    }

    // �U������I�u�W�F�N�g��L���ɂ���
    const auto& attack_object = owner->FindChildObject(PlayerConstant::SPIN_ATTACK_OBJECT_NAME);  // �q�I�u�W�F�N�g(�U���p�I�u�W�F�N�g)�擾
    if (!attack_object) return;
    auto collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
    if (!collision) return;
        
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

    // �U������I�u�W�F�N�g��L���ɂ���
    const auto& attack_object = owner->FindChildObject(PlayerConstant::SPIN_ATTACK_OBJECT_NAME);  // �q�I�u�W�F�N�g(�U���p�I�u�W�F�N�g)�擾
    if (!attack_object) return;
    auto collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
    if (!collision) return;
    collision->EvaluateCollision();

    if (const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr); character.get())
    {
        // ���S����
        if (!character->IsAlive())
        {
            // ��_���X�e�[�g�ɑJ��
            state_machine->ChangeState(this->change_dead_state);
            return;
        }
    }

    // �A�j���[�V�����Đ��҂�
    if (!animation->IsPlayAnimation())
    {
        // �ҋ@�X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_idle_state);
        return;
    }
}

void PlayerSpinAttackState::End()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // �v���C���[�̓��͈ړ���L���ɂ���
    auto player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (player)
    {
        player->SetInputMoveValidityFlag(true);
        player->SetMoveRate(1.0f);
    }

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
    this->change_spin_attack_state.change_state_name = PlayerSpinAttackSpinLoopState::STATE_NAME;
    this->change_dead_state.change_state_name = PlayerDeadState::STATE_NAME;
}

void PlayerDamageState::Start()
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

    if (const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr); character.get())
    {
        // ���S����
        if (!character->IsAlive())
        {
            // ��_���X�e�[�g�ɑJ��
            state_machine->ChangeState(this->change_dead_state);
            return;
        }
    }

    // �A�j���[�V�����Đ��҂�
    if (!animation->IsPlayAnimation())
    {
        // �ҋ@�X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_idle_state);
        return;
    }
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
    this->change_dead_idle_state.change_state_name = PlayerDeadIdleState::STATE_NAME;
}

void PlayerDeadState::Start()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    // �A�j���[�V�����Đ�
    const auto& animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (animation)
    {
        animation->PlayAnimation(PlayerConstant::ANIMATION::DEAD, false);
    }
    // ���͎�t�����Ȃ��悤�ɐݒ�
    const auto& player = owner->GetComponent<PlayerComponent>(this->player_Wprt);
    if (player)
    {
        player->SetInputMoveValidityFlag(false);
    }
}

void PlayerDeadState::Update(float elapsed_time)
{   
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    const auto& animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    
    // �A�j���[�V�����Đ��҂�
    if (!animation->IsPlayAnimation())
    {
        // ���S�ҋ@�X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_dead_idle_state);
        return;
    }
}

// ���S�X�e�[�g
const MyHash PlayerDeadIdleState::STATE_NAME = MyHash("PlayerDeadIdleState");
PlayerDeadIdleState::PlayerDeadIdleState()
    :State(STATE_NAME)
{
}

void PlayerDeadIdleState::Start()
{
    // �A�j���[�V�����Đ�
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    animation->PlayAnimation(PlayerConstant::ANIMATION::DEAD_STAY, false);

    // �Q�[�����[�h��ݒ�
    if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
    {
        // �s�k��ԂɕύX
        game_data->SetGameStatus(GameData::GameStatus::DEFEAT);
    }
}
