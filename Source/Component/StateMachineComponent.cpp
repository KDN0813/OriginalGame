#include "System/Misc.h"
#include "StateMachineComponent.h"

void StateMachineComponent::Update(float elapsed_time)
{
    this->current_state->Execute(elapsed_time);
}

void StateMachineComponent::ChangeState(State::ChangeState& chage_state)
{
	// 無効なインデックスの場合
	if (chage_state.change_state_index == State::INVALID_STATE_INDEX)
	{
		// ステートのインデックスを名前検索する
		size_t index = FindStateIndex(chage_state.change_state_name);
		if (index == State::INVALID_STATE_INDEX) return;	// 無効な値ならreturn
		chage_state.change_state_index = index;
	}

	// 現在のステートのExit関数を実行
	this->current_state->Exit();

	// 新しいステートをセット
	this->current_state = this->state_pool.at(chage_state.change_state_index).get();

	// 新しいステートのEnter関数を呼び出す。
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
	_ASSERT_EXPR_W((state != nullptr), L"ステートが存在しません");
	if (!state) return State::INVALID_STATE_INDEX;
	return state->GetStateIndex();
}
