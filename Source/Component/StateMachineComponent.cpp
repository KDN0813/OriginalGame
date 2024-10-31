#include "StateMachineComponent.h"

void StateMachineComponent::Update(float elapsed_time)
{
    current_state->Execute(elapsed_time);
}

void StateMachineComponent::ChangeState(int newState)
{
	// ���݂̃X�e�[�g��Exit�֐������s
	current_state->Exit();

	// �V�����X�e�[�g���Z�b�g
	current_state = state_pool.at(newState).get();

	// �V�����X�e�[�g��Enter�֐����Ăяo���B
	current_state->Enter();
}

int StateMachineComponent::GetStateIndex()
{
    return 0;
}
