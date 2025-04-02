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

PlayerIdleState::PlayerIdleState()
{
}

void PlayerIdleState::Start()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    animation->PlayMainPartsAnimation(PlayerConstant::ANIMATION::IDLE, true, 0.2f);
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
        state_machine->ChangeState("MoveState");
        return;
    }

    if (const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr); character.get())
    {
        // ���S����
        if (!character->IsAlive())
        {
            // ��_���X�e�[�g�ɑJ��
            state_machine->ChangeState("DeadState");
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
            state_machine->ChangeState("AttackState");
        }
        // Y�{�^��
        if (pad.GetButtonDown() & GamePad::BTN_Y)
        {
            // ��]�U���X�e�[�g�ɑJ��
            if (player->IsUseSpecialGage(player->GetSpinAttackUseGageCount()))
            {
                state_machine->ChangeState("SpinAttackState");
                return;
            }
        }
    }
}

PlayerMoveState::PlayerMoveState()
{
}

void PlayerMoveState::Start()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    animation->PlayMainPartsAnimation(PlayerConstant::ANIMATION::MOVE_FWD, true, 0.2f);
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
        state_machine->ChangeState("IdleState");
        return;
    }

    if (const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr); character.get())
    {
        // ���S����
        if (!character->IsAlive())
        {
            // ��_���X�e�[�g�ɑJ��
            state_machine->ChangeState("DeadState");
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
            state_machine->ChangeState("AttackState");
            return;
        }
        // Y�{�^��
        if (pad.GetButtonDown() & GamePad::BTN_Y)
        {
            // ��]�U���X�e�[�g�ɑJ��
            if (player->IsUseSpecialGage(player->GetSpinAttackUseGageCount()))
            {
                state_machine->ChangeState("SpinAttackState");
                return;
            }
        }
    }
}

PlayerAttackState::PlayerAttackState()
{
}

void PlayerAttackState::Start()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // �A�j���[�V�����̍Đ�
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (animation)
        animation->PlayMainPartsAnimation(PlayerConstant::ANIMATION::ATTACK01, false, 0.2f);

    // SE�Đ�
    if (Audio::Instance audio = Audio::GetInstance(); audio.Get())
    {
        AudioParam param{};
        param.volume = 0.5f;
        param.loop = false;
        param.filename = "Data/Audio/SE_Slash01.wav";
        audio->Play(param);
    }

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
            state_machine->ChangeState("DeadState");
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
            state_machine->ChangeState("AttackComboState");
            return;
        }
    }

    // �A�j���[�V�����Đ��҂�
    if (player->attack_end_point <= animation->GetMainPartsCurrentAnimationSeconds())
    {
        // �ҋ@�X�e�[�g�ɑJ��
        state_machine->ChangeState("AttackHoldState");
        return;
    }
}

void PlayerAttackState::End()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

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

PlayerAttackComboState::PlayerAttackComboState()
{
}

void PlayerAttackComboState::Start()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // �A�j���[�V�����̍Đ�
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (animation)
        animation->PlayMainPartsAnimation(PlayerConstant::ANIMATION::ATTACK02, false, 0.2f);

    // SE�Đ�
    if (Audio::Instance audio = Audio::GetInstance(); audio.Get())
    {
        AudioParam param{};
        param.volume = 0.5f;
        param.loop = false;
        param.filename = "Data/Audio/SE_Slash01.wav";
        audio->Play(param);
    }

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

void PlayerAttackComboState::Update(float elapsed_time)
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
            state_machine->ChangeState("DeadState");
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
            state_machine->ChangeState("AttackState");
            return;
        }
    }

    // �A�j���[�V�����Đ��҂�
    if (player->attack_combo2_end_point <= animation->GetMainPartsCurrentAnimationSeconds())
    {
        // �ҋ@�X�e�[�g�ɑJ��
        state_machine->ChangeState("AttackHoldState");
        return;
    }
}

void PlayerAttackComboState::End()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

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

PlayerAttackHoldState::PlayerAttackHoldState()
{
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
            state_machine->ChangeState("DeadState");
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
                state_machine->ChangeState("SpinAttackState");
                return;
            }
        }
    }

    // �ړ�����
    auto movement = owner->GetComponent<MovementComponent>(this->movement_Wpt);
    if (!movement) return;
    if (movement->IsMoveXZAxis())
    {
        state_machine->ChangeState("MoveState");
        return;
    }

    // �A�j���[�V�����Đ��҂�
    if (!animation->IsPlayMainPartsAnimation())
    {
        // �ҋ@�X�e�[�g�ɑJ��
        state_machine->ChangeState("IdleState");
        return;
    }
}

PlayerSpinAttackStartState::PlayerSpinAttackStartState()
{
}

void PlayerSpinAttackStartState::Start()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    // �A�j���[�V�����̍Đ�
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (animation)
        animation->PlayMainPartsAnimation(PlayerConstant::ANIMATION::SPIN_ATTACK_START, false, 0.2f);
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
    if (!animation->IsPlayMainPartsAnimation())
    {
        // ��]
        state_machine->ChangeState("SpinAttackState");
        return;
    }
}

PlayerSpinAttackSpinLoopState::PlayerSpinAttackSpinLoopState()
{
}

void PlayerSpinAttackSpinLoopState::Start()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // �A�j���[�V�����̍Đ�
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (animation)
        animation->PlayMainPartsAnimation(PlayerConstant::ANIMATION::SPIN_ATTACK_SPIN_LOOP, true, 0.0f);

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
        player->SetSpinAttackTimer(player->GetSpinAttackTime());

        // ��]�U���̎c�莞��UI���A�N�e�B�u������
        if (const auto& player_owner = player->GetOwner())
        {
            if (const auto& spin_attack_ui_object = player_owner->FindChildObject(PlayerConstant::SPIN_ATTACK_TIME_UI_NAME))
            {
                spin_attack_ui_object->SetIsActive(true);
            }
        }
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
            state_machine->ChangeState("DeadState");
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

    float attack_time = player->GetSpinAttackTimer() - elapsed_time;
    if (attack_time <= 0.0f)
    {
        // �U�����Ԃ�0�ɂȂ�����ҋ@�X�e�[�g�ɑJ��
        state_machine->ChangeState("IdleState");
    }
    player->SetSpinAttackTimer(attack_time);
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

        // ��]�U���̎c�莞��UI���A�N�e�B�u������
        if (const auto& player_owner = player->GetOwner())
        {
            if (const auto& spin_attack_ui_object = player_owner->FindChildObject(PlayerConstant::SPIN_ATTACK_TIME_UI_NAME))
            {
                spin_attack_ui_object->SetIsActive(false);
            }
        }
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

PlayerSpinAttackState::PlayerSpinAttackState()
{
}

void PlayerSpinAttackState::Start()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;

    // �A�j���[�V�����̍Đ�
    auto animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (animation)
        animation->PlayMainPartsAnimation(PlayerConstant::ANIMATION::SPIN_ATTACK, false, 0.0f);

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
            state_machine->ChangeState("DeadState");
            return;
        }
    }

    // �A�j���[�V�����Đ��҂�
    if (!animation->IsPlayMainPartsAnimation())
    {
        // �ҋ@�X�e�[�g�ɑJ��
        state_machine->ChangeState("IdleState");
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

PlayerDamageState::PlayerDamageState()
{
}

void PlayerDamageState::Start()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    animation->PlayMainPartsAnimation(PlayerConstant::ANIMATION::DAMAGE, false);

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
            state_machine->ChangeState("DeadState");
            return;
        }
    }

    // �A�j���[�V�����Đ��҂�
    if (!animation->IsPlayMainPartsAnimation())
    {
        // �ҋ@�X�e�[�g�ɑJ��
        state_machine->ChangeState("IdleState");
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

PlayerDeadState::PlayerDeadState()
{
}

void PlayerDeadState::Start()
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    // �A�j���[�V�����Đ�
    const auto& animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (animation)
    {
        animation->PlayMainPartsAnimation(PlayerConstant::ANIMATION::DEAD, false);
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
    if (!animation->IsPlayMainPartsAnimation())
    {
        // ���S�ҋ@�X�e�[�g�ɑJ��
        state_machine->ChangeState("DeadIdleState");
        return;
    }
}

// ���S�X�e�[�g
PlayerDeadIdleState::PlayerDeadIdleState()
{
}

void PlayerDeadIdleState::Start()
{
    // �A�j���[�V�����Đ�
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& animation = owner->GetComponent<ModelAnimationControlComponent>(this->animation_Wprt);
    if (!animation) return;
    animation->PlayMainPartsAnimation(PlayerConstant::ANIMATION::DEAD_STAY, false);

    // �Q�[�����[�h��ݒ�
    if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
    {
        // �s�k��ԂɕύX
        game_data->SetGameStatus(GameData::GameStatus::DEFEAT);
    }
}
