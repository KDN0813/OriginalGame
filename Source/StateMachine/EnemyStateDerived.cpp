#include "EnemyStateDerived.h"
#include "Object\Object.h"
#include "Object\Constant\EnemyConstant.h"
#include "System\MyMath\MyMathf.h"
#include "System\MyMath\MYVECTOR3.h"

#include "Component\StateMachineComponent.h"
#include "Component\EnemyComponent.h"
#include "Component\ModelAnimationControlComponent.h"
#include "Component\TransformComponent.h"

const MyHash EnemyIdleState::STATE_NAME = MyHash("EnemydleState");
EnemyIdleState::EnemyIdleState()
    :State(STATE_NAME)
{
    this->change_wandering_state.change_state_name = EnemyWanderingState::STATE_NAME;
    this->change_chase_state.change_state_name = EnemyChaseState::STATE_NAME;
    this->change_attack_state.change_state_name = EnemyAttackState::STATE_NAME;
    this->change_damage_state.change_state_name = EnemyDamageState::STATE_NAME;
    this->change_deth_state.change_state_name = EnemyDethState::STATE_NAME;
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
    }
}

void EnemyIdleState::SetRandomIdleTime()
{
    this->idle_timer = MyMathf::RandomRange(EnemyConstant::MIN_IDLE_TIME, EnemyConstant::MAX_IDLE_TIME);
}

const MyHash EnemyWanderingState::STATE_NAME = MyHash("EnemyWanderingState");
EnemyWanderingState::EnemyWanderingState()
    :State(STATE_NAME)
{
    this->change_idle_state.change_state_name = EnemyIdleState::STATE_NAME;
    this->change_chase_state.change_state_name = EnemyChaseState::STATE_NAME;
    this->change_attack_state.change_state_name = EnemyAttackState::STATE_NAME;
    this->change_damage_state.change_state_name = EnemyDamageState::STATE_NAME;
    this->change_deth_state.change_state_name = EnemyDethState::STATE_NAME;
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
}

void EnemyWanderingState::Update(float elapsed_time)
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& enemy = owner->GetComponent<EnemyComponent>(this->enemy_Wptr);
    if (!enemy) return;
    const auto& state_machine = owner->GetComponent<StateMachineComponent>(this->state_machine_Wptr);
    if (!state_machine) return;

    if (enemy->IsAtTarget())
    {
        // �ړI�n�ɓ���������ҋ@��ԂɑJ��
        state_machine->ChangeState(this->change_idle_state);
    }

    // �ړ��͈͂Ƀv���C���[�����݂��邩����
    if (enemy->IsPlayerInMovementArea())
    {
        // �͈͓��ɑ��݂���ΐڋ߃X�e�[�g�ɑJ��
        state_machine->ChangeState(this->change_chase_state);
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
    this->change_idle_state.change_state_name = EnemyIdleState::STATE_NAME;
}

const MyHash EnemyAttackState::STATE_NAME = MyHash("EnemyAttackState");
EnemyAttackState::EnemyAttackState()
    :State(STATE_NAME)
{
    this->change_idle_state.change_state_name = EnemyIdleState::STATE_NAME;
}

const MyHash EnemyDamageState::STATE_NAME = MyHash("EnemyDamageState");
EnemyDamageState::EnemyDamageState()
    :State(STATE_NAME)
{
    this->change_idle_state.change_state_name = EnemyIdleState::STATE_NAME;
}

const MyHash EnemyDethState::STATE_NAME = MyHash("EnemyDethState");
EnemyDethState::EnemyDethState()
    :State(STATE_NAME)
{
    this->change_deth_idle_state.change_state_name = EnemyDethIdleState::STATE_NAME;
}

const MyHash EnemyDethIdleState::STATE_NAME = MyHash("EnemyDethIdleState");
EnemyDethIdleState::EnemyDethIdleState()
    :State(STATE_NAME)
{
}
