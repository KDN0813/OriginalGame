#include "EnemyStateDerived.h"
#include <cmath>
#include "Object\Object.h"
#include "Object\GameObject.h"
#include "Object\Constant\EnemyConstant.h"
#include "System\MyMath\MyMathf.h"
#include "System\MyMath\MYVECTOR3.h"
#include "System\GameData.h"

#include "Component\StateMachineComponent.h"
#include "Component\EnemyComponent.h"
#include "Component\InstancedModelWithAnimationComponent.h"
#include "Component\TransformComponent.h"
#include "Component\CircleCollisionComponent.h"
#include "Component\CharacterComponent.h"
#include "Component\PlayerComponent.h"

const MyHash EnemyIdleState::STATE_NAME = MyHash("EnemyIdleState");
EnemyIdleState::EnemyIdleState()
    :State(STATE_NAME)
{
    this->change_wandering_state.change_state_name = EnemyWanderingState::STATE_NAME;
    this->change_chase_state.change_state_name = EnemyChaseState::STATE_NAME;
    this->change_attack_state.change_state_name = EnemyAttackState::STATE_NAME;
    this->change_damage_state.change_state_name = EnemyDamageState::STATE_NAME;
    this->change_deth_state.change_state_name = EnemyDeadState::STATE_NAME;
}

void EnemyIdleState::Start()
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& animation = owner->GetComponent(animation_Wprt);
    if (!animation)return;
    // �A�j���[�V�����Đ�
    animation->PlayAnimation(EnemyConstant::IDLE_NORMAL, true);

    // �ҋ@���Ԑݒ�
    SetRandomIdleTime();
}

void EnemyIdleState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    const auto& enemy = owner->GetComponent<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!state_machine) return;

    if (this->idle_timer > 0.0f)
    {
        // �ҋ@�^�C�}�[�X�V
        this->idle_timer -= elapsed_time;
    }
    else
    {
        // �ҋ@���Ԃ��I��������ړ���ԂɑJ��
        state_machine->ChangeState(this->change_wandering_state);
        return;
    }

    // �ړ��͈͂Ƀv���C���[�����݂��邩����
    if (enemy->IsPlayerInMovementArea())
    {
        // �͈͓��ɑ��݂���ΐڋ߃X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_chase_state);
        return;
    }

    // �_���[�W���󂯂�������
    if (character->IsDamage())
    {
        // �_���[�W���󂯂��Ȃ�_���[�W�X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_damage_state);
        return;
    }

    // �������Ă邩����
    if (!character->IsAlive())
    {
        // �������Ă��Ȃ��Ȃ玀�S�X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_deth_state);
        return;
    }
}

void EnemyIdleState::SetRandomIdleTime()
{
    this->idle_timer = MyMath::RandomRange(EnemyConstant::MIN_IDLE_TIME, EnemyConstant::MAX_IDLE_TIME);
}

const MyHash EnemyWanderingState::STATE_NAME = MyHash("EnemyWanderingState");
EnemyWanderingState::EnemyWanderingState()
    :State(STATE_NAME)
{
    this->change_idle_state.change_state_name = EnemyIdleState::STATE_NAME;
    this->change_chase_state.change_state_name = EnemyChaseState::STATE_NAME;
    this->change_attack_state.change_state_name = EnemyAttackState::STATE_NAME;
    this->change_damage_state.change_state_name = EnemyDamageState::STATE_NAME;
    this->change_deth_state.change_state_name = EnemyDeadState::STATE_NAME;
}

void EnemyWanderingState::Start()
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& animation = owner->GetComponent(animation_Wprt);
    if (!animation)return;
    // �A�j���[�V�����Đ�
    animation->PlayAnimation(EnemyConstant::MOVE_FWD, true);

    const auto& enemy = owner->GetComponent<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
    // �ړ��L���ɐݒ�
    enemy->SetMoveValidityFlag(true);
    // �^�[�Q�b�g�ݒ�
    enemy->SetRandomTargetPosition();
}

void EnemyWanderingState::Update(float elapsed_time)
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& enemy = owner->GetComponent<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!state_machine) return;

    if (enemy->IsAtTarget())
    {
        // �ړI�n�ɓ���������ҋ@��ԂɑJ��
        state_machine->ChangeState(this->change_idle_state);
        return;
    }

    // �ړ��͈͂Ƀv���C���[�����݂��邩����
    if (enemy->IsPlayerInMovementArea())
    {
        // �͈͓��ɑ��݂���ΐڋ߃X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_chase_state);
        return;
    }

    // �_���[�W���󂯂�������
    if (character->IsDamage())
    {
        // �_���[�W���󂯂��Ȃ�_���[�W�X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_damage_state);
        return;
    }

    // �������Ă邩����
    if (!character->IsAlive())
    {
        // �������Ă��Ȃ��Ȃ玀�S�X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_deth_state);
        return;
    }
}

void EnemyWanderingState::End()
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& enemy = owner->GetComponent<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
    // �ړ������ɐݒ�
    enemy->SetMoveValidityFlag(false);
}

const MyHash EnemyChaseState::STATE_NAME = MyHash("EnemyChaseState");
EnemyChaseState::EnemyChaseState()
    :State(STATE_NAME)
{
    this->change_attack_state.change_state_name = EnemyAttackState::STATE_NAME;
    this->change_idle_state.change_state_name = EnemyIdleState::STATE_NAME;
    this->change_damage_state.change_state_name = EnemyDamageState::STATE_NAME;
    this->change_deth_state.change_state_name = EnemyDeadState::STATE_NAME;
}

void EnemyChaseState::Start()
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& animation = owner->GetComponent(animation_Wprt);
    if (!animation)return;
    // �A�j���[�V�����Đ�
    animation->PlayAnimation(EnemyConstant::MOVE_FWD, true);

    const auto& enemy = owner->GetComponent<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
    // �ړ��L���ɐݒ�
    enemy->SetMoveValidityFlag(true);
}

void EnemyChaseState::Update(float elapsed_time)
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& enemy = owner->GetComponent<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!state_machine) return;

    // �ړI�n���v���C���[�ɐݒ�
    enemy->SetTargetPositionByPlayer();

    // �U���͈͂Ƀv���C���[�����݂��邩
#if 0   // �G�̏d�Ȃ��h�������̂��߈ꎞ�I�ɏ������X�L�b�v���Ă܂�
    if (enemy->IsPlayerInAttaclArea())
    {
        // �͈͓��ɂ���Ȃ�U���X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_attack_state);
        return;
    }
#endif // 0

    // �ړ��͈͂Ƀv���C���[�����݂��邩����
    if (!enemy->IsPlayerInMovementArea())
    {
        // �͈͓��ɂ��Ȃ��Ȃ�ҋ@�X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_idle_state);
        return;
    }

    // �_���[�W���󂯂�������
    if (character->IsDamage())
    {
        // �_���[�W���󂯂��Ȃ�_���[�W�X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_damage_state);
        return;
    }

    // �������Ă邩����
    if (!character->IsAlive())
    {
        // �������Ă��Ȃ��Ȃ玀�S�X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_deth_state);
        return;
    }
}

void EnemyChaseState::End()
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& enemy = owner->GetComponent<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
    // �ړ������ɐݒ�
    enemy->SetMoveValidityFlag(false);
}

const MyHash EnemyAttackState::STATE_NAME = MyHash("EnemyAttackState");
EnemyAttackState::EnemyAttackState()
    :State(STATE_NAME)
{
    this->change_idle_state.change_state_name = EnemyIdleState::STATE_NAME;
    this->change_chase_state.change_state_name = EnemyChaseState::STATE_NAME;
    this->change_damage_state.change_state_name = EnemyDamageState::STATE_NAME;
    this->change_deth_state.change_state_name = EnemyDeadState::STATE_NAME;
}

void EnemyAttackState::Start()
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& animation = owner->GetComponent(animation_Wprt);
    if (!animation)return;
    // �A�j���[�V�����Đ�
    animation->PlayAnimation(EnemyConstant::ATTACK01, false);

    // �U������I�u�W�F�N�g��L���ɂ���
    const auto& attack_object = owner->FindChildObject(EnemyConstant::ATTACK_OBJECT_NAME);  // �q�I�u�W�F�N�g(�U���p�I�u�W�F�N�g)�擾
    if (!attack_object) return;
    auto collision = attack_object->GetComponent<CircleCollisionComponent>(this->child_collision_Wprt);
    if (collision)
        collision->SetIsActive(true);  // �R���W������L���ɂ���

    collision->EvaluateCollision();
}

void EnemyAttackState::Update(float elapsed_time)
// �ړ�����
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& animation = owner->GetComponent(animation_Wprt);
    if (!animation)return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    const auto& enemy = owner->GetComponent<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!state_machine) return;

    // �A�j���[�V�����Đ����ł��邩
    if (!animation->IsPlayAnime())
    {
        // �Đ����I����Ă�����

        if (enemy->IsPlayerInMovementArea())
        {
            // �͈͓��ɑ��݂���ΐڋ߃X�e�[�g�ɑJ��
            state_machine->ChangeState(this->change_chase_state);
            return;
        }
        else
        {
            // �͈͓��ɂ��Ȃ��Ȃ�ҋ@�X�e�[�g�ɑJ��
            state_machine->ChangeState(this->change_idle_state);
            return;
        }
    }

    // �_���[�W���󂯂�������
    if (character->IsDamage())
    {
        // �_���[�W���󂯂��Ȃ�_���[�W�X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_damage_state);
        return;
    }

    // �������Ă邩����
    if (!character->IsAlive())
    {
        // �������Ă��Ȃ��Ȃ玀�S�X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_deth_state);
        return;
    }
}

void EnemyAttackState::End()
{
}

const MyHash EnemyDamageState::STATE_NAME = MyHash("EnemyDamageState");
EnemyDamageState::EnemyDamageState()
    :State(STATE_NAME)
{
    this->change_idle_state.change_state_name = EnemyIdleState::STATE_NAME;
    this->change_deth_state.change_state_name = EnemyDeadState::STATE_NAME;
}

void EnemyDamageState::Start()
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& animation = owner->GetComponent(animation_Wprt);
    if (!animation)return;
    // �A�j���[�V�����Đ�
    animation->PlayAnimation(EnemyConstant::TAUNTING, false);
}

void EnemyDamageState::Update(float elapsed_time)
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& animation = owner->GetComponent(animation_Wprt);
    if (!animation)return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!state_machine) return;

    if (!animation->IsPlayAnime())
    {
        // �A�j���[�V�������I���ҋ@��ԂɑJ��
        state_machine->ChangeState(this->change_idle_state);
        return;
    }

    // �������Ă邩����
    if (!character->IsAlive())
    {
        // �������Ă��Ȃ��Ȃ玀�S�X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_deth_state);
        return;
    }
}

const MyHash EnemyDeadState::STATE_NAME = MyHash("EnemyDeadState");
EnemyDeadState::EnemyDeadState()
    :State(STATE_NAME)
{
    this->change_deth_idle_state.change_state_name = EnemyDeadIdleState::STATE_NAME;
}

void EnemyDeadState::Start()
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& animation = owner->GetComponent(animation_Wprt);
    if (!animation)return;
    const auto& enemy = owner->GetComponent<EnemyComponent>(enemy_Wptr);
    if (!enemy)return;

    // �A�j���[�V�����Đ�
    animation->PlayAnimation(EnemyConstant::DIE, false);

    // ���S�҂��t���O�𗧂Ă�
    enemy->SetPendingRemovalFlag(true);
}

void EnemyDeadState::Update(float elapsed_time)
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& animation = owner->GetComponent(animation_Wprt);
    if (!animation)return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;

    if (!animation->IsPlayAnime())
    {
        // �A�j���[�V�������I���ҋ@��ԂɑJ��
        state_machine->ChangeState(this->change_deth_idle_state);
        return;
    }
}

const MyHash EnemyDeadIdleState::STATE_NAME = MyHash("EnemyDeadIdleState");
EnemyDeadIdleState::EnemyDeadIdleState()
    :State(STATE_NAME)
{
}

void EnemyDeadIdleState::Start()
{
    this->remove_timer = EnemyConstant::REMOVE_IDLE_TIME;
}

void EnemyDeadIdleState::Update(float elapsed_time)
{
    const auto& owner = GetOwner();
    if (!owner)return;

    if (this->remove_timer > 0.0f)
    {
        // �폜�^�C�}�[�X�V
        this->remove_timer -= elapsed_time;
    }
    else
    {
        owner->SetIsRemove(true);   // �폜����
    }
}

const MyHash EnemySpawnState::STATE_NAME = MyHash("EnemySpawnState");
EnemySpawnState::EnemySpawnState()
    :State(STATE_NAME)
{
    this->change_chase_state.change_state_name = EnemyChaseState::STATE_NAME;
}

void EnemySpawnState::Start()
{
    this->idle_timer = 0.0f;    // �^�C�}�[���Z�b�g

    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& animation = owner->GetComponent(animation_Wprt);
    if (!animation)return;
    // �A�j���[�V�����Đ�
    animation->PlayAnimation(EnemyConstant::IDLE_NORMAL, true);

    // alpha�l��0�ɐݒ�
    animation->SetAlpha(0.0f);

    // ���G��Ԃɐݒ�
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    character->SetInvincibleFlag(true);

    // �ړ���~
    const auto& enemy = owner->GetComponent(enemy_Wptr);
    if (!enemy) return;
    enemy->SetMoveValidityFlag(false);
}

void EnemySpawnState::Update(float elapsed_time)
{
    const auto& owner = this->GetOwner();
    if (!owner) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;
    const auto& enemy = owner->GetComponent<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
    const auto& animation = owner->GetComponent(animation_Wprt);
    if (!animation)return;

    // ���f���̓����x�ݒ�
    const float alpha = std::lerp(0.0f, 1.0f, this->idle_timer / this->IDLE_TIME);

    if (this->idle_timer < this->IDLE_TIME)
    {
        // �ҋ@�^�C�}�[�X�V
        this->idle_timer += elapsed_time;
    }
    else
    {
        // �ҋ@���Ԃ��I��������ړ���ԂɑJ��
        state_machine->ChangeState(this->change_chase_state);
        animation->SetAlpha(1.0f);
        return;
    }

    animation->SetAlpha(alpha);
}

void EnemySpawnState::End()
{
    const auto& owner = GetOwner();
    if (!owner)return;

    // �ړ�����
    const auto& enemy = owner->GetComponent(enemy_Wptr);
    if (!enemy) return;
    enemy->SetMoveValidityFlag(false);

    // ���G��ԉ���
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    character->SetInvincibleFlag(false);
}
