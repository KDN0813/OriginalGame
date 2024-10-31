#include "StateMachineComponent.h"

void StateMachineComponent::Update(float elapsed_time)
{
    current_state->Execute(elapsed_time);
}

void StateMachineComponent::ChangeState(int newState)
{
	// 現在のステートのExit関数を実行
	current_state->Exit();

	// 新しいステートをセット
	current_state = state_pool.at(newState).get();

	// 新しいステートのEnter関数を呼び出す。
	current_state->Enter();
}

int StateMachineComponent::GetStateIndex()
{
    return 0;
}
