#include "EnemyStateDerived.h"

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
}

void EnemyIdleState::Update(float elapsed_time)
{
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
