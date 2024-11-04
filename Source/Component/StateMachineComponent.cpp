#include "System/Misc.h"
#ifdef _DEBUG
#include "Debug/ImGuiHelper.h"
#endif // DEBUG
#include "StateMachineComponent.h"

void StateMachineComponent::Update(float elapsed_time)
{
    this->current_state->Update(elapsed_time);
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

	// ���݂̃X�e�[�g�̏I���֐������s
	this->current_state->End();

	// �V�����X�e�[�g���Z�b�g
	this->current_state = this->state_pool.at(chage_state.change_state_index).get();

	// �V�����X�e�[�g�̊J�n�֐����Ăяo���B
	this->current_state->Staet();
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

#ifdef _DEBUG

void StateMachineComponent::DrawDebugGUI()
{
	State::StateIndex current_state_index = this->current_state->GetStateIndex();	// ���݂̃X�e�[�g�̃C���f�b�N�X
	ImGui::InputSize_t("State Index", current_state_index);

	std::string current_state_name = this->state_name_pool[static_cast<size_t>(current_state_index)];	// ���݂̃X�e�[�g��
	int current_index_int = static_cast<int>(current_state_index);
	if (ImGui::ComboUI("State", current_state_name, this->state_name_pool, current_index_int))
	{
		this->state_pool[current_state_index]->End();
		this->current_state = this->state_pool.at(static_cast<size_t>(current_index_int)).get();
		this->state_pool[current_state_index]->Staet();
	}
}

#endif // DEBUG