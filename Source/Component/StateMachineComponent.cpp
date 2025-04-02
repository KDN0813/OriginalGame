#include "System/Misc.h"
#ifdef _DEBUG
#include "Debug/ImGuiHelper.h"
#endif // DEBUG
#include "StateMachineComponent.h"

StateMachineComponent::StateMachineComponent()
{
}

void StateMachineComponent::Start()
{
	ChangeState(default_state_key);
}

void StateMachineComponent::ReStart()
{
	ChangeState(default_state_key);
}

void StateMachineComponent::Update(float elapsed_time)
{
    this->current_state->Update(elapsed_time);
}

void StateMachineComponent::ChangeState(std::string state_key)
{
	const auto& next_state = this->state_pool[state_key];
	if (!next_state) return;

	// ���݂̃X�e�[�g�̏I���֐������s
	if (this->current_state) this->current_state->End();

	// �V�����X�e�[�g���Z�b�g
	this->current_state = next_state.get();

	// �V�����X�e�[�g�̊J�n�֐����Ăяo���B
	this->current_state->Start();
}

void StateMachineComponent::SetDefaultState(std::string default_state_key)
{
	this->default_state_key = default_state_key;
}

State* StateMachineComponent::FindState(std::string state_key)
{
	const auto& next_state = this->state_pool[state_key];
	if (!next_state) return nullptr;
	return next_state.get();
}

#ifdef _DEBUG

void StateMachineComponent::DrawDebugGUI()
{
	//State::StateIndex current_state_index = this->current_state->GetStateIndex();	// ���݂̃X�e�[�g�̃C���f�b�N�X
	//std::string current_state_name = this->state_name_pool[static_cast<size_t>(current_state_index)];	// ���݂̃X�e�[�g��
	//int current_index_int = static_cast<int>(current_state_index);
	//if (ImGui::ComboUI("State", current_state_name, this->state_name_pool, current_index_int))
	//{
	//	current_state->End();
	//	this->current_state = this->state_pool[this->state_name_pool[(static_cast<size_t>(current_index_int))]].get();
	//	current_state->Start();
	//}
}

#endif // DEBUG