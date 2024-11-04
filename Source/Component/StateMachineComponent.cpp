#include "System/Misc.h"
#include "StateMachineComponent.h"

void StateMachineComponent::Update(float elapsed_time)
{
    this->current_state->Execute(elapsed_time);
}

void StateMachineComponent::ChangeState(State::ChangeState& chage_state)
{
	// �����ȃC���f�b�N�X�̏ꍇ
	if (chage_state.change_state_index == State::INVALID_STATE_INDEX)
	{
		// �X�e�[�g�̃C���f�b�N�X�𖼑O��������
		size_t index = FindStateIndex(chage_state.change_state_name);
		if (index == State::INVALID_STATE_INDEX) return;	// �����Ȓl�Ȃ�return
		chage_state.change_state_index = index;
	}

	// ���݂̃X�e�[�g��Exit�֐������s
	this->current_state->Exit();

	// �V�����X�e�[�g���Z�b�g
	this->current_state = this->state_pool.at(chage_state.change_state_index).get();

	// �V�����X�e�[�g��Enter�֐����Ăяo���B
	this->current_state->Enter();
}

State* StateMachineComponent::FindState(MyHash name)
{
	for (size_t i = 0; i < this->state_pool.size(); ++i)
	{
		State* state = this->state_pool[i].get();
		if (state->GetHash().PerfectEqual(name))
		{
			return state;
		}
	}
	return nullptr;
}

State::StateIndex StateMachineComponent::FindStateIndex(MyHash name)
{
	State* state = FindState(name);
	_ASSERT_EXPR_W((state != nullptr), L"�X�e�[�g�����݂��܂���");
	if (!state) return State::INVALID_STATE_INDEX;
	return state->GetStateIndex();
}
