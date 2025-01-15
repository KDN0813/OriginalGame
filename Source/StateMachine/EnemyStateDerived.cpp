#include "EnemyStateDerived.h"

const MyHash EnemydleState::STATE_NAME = MyHash("EnemydleState");
EnemydleState::EnemydleState()
    :State(STATE_NAME)
{
}

void EnemydleState::Start()
{
}

void EnemydleState::Update(float elapsed_time)
{
}
