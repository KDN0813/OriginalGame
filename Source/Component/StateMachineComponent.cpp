#include "Debug/ImGuiHelper.h"
#include "StateMachineComponent.h"
#include "StateMachine/TransitionJudgement.h"

StateMachineComponent::StateMachineComponent()
{
}

void StateMachineComponent::Start()
{
    this->state_index = (this->default_state == INVALID_STATE_INDEX) ? 0 : this->default_state;
    this->next_state = this->state_index;
    this->state_pool[this->state_index]->Start();
}

void StateMachineComponent::End()
{
}

void StateMachineComponent::Update(float elapsed_time)
{
    if (this->state_index < 0 || this->state_index == INVALID_STATE_INDEX || this->state_index >= this->state_pool.size()) return;

    StateBase* state = this->state_pool[this->state_index].get();

    PreTransitionJudgemen(state);
    state->Update(elapsed_time);
    PostTransitionJudgemen(state);

    if (this->state_index != this->next_state) 
    {
        this->state_pool[this->state_index]->End();
        this->state_index = this->next_state;
        this->state_pool[this->state_index]->Start();
    }
}

void StateMachineComponent::ChangeState(StateIndex state_index)
{
    if (state_index < 0 || state_index == INVALID_STATE_INDEX || state_index >= this->state_pool.size()) return;
    this->next_state = state_index;
}

void StateMachineComponent::SetDefaultState(StateIndex state_index)
{
    if (state_index < 0 || state_index == INVALID_STATE_INDEX || state_index >= this->state_pool.size()) return;
    this->default_state = state_index;
}

void StateMachineComponent::SetDefaultState(std::string state_name)
{
    SetDefaultState(MyHash(state_name));
}

void StateMachineComponent::SetDefaultState(MyHash state_name)
{
    SetDefaultState(FindStateIndex(state_name));
}

void StateMachineComponent::PreTransitionJudgemen(StateBase* state)
{
    if (!state) return;
    for (auto& state_jugement : state->GetPreUpdateJudgementPool())
    {
        if (state->PerformTransitionJudgement(state_jugement->GetJudgement()))
        {
            StateIndex next_index = state_jugement->GetNextStateIndex();
            if (next_index <= this->state_pool.size()) continue;
            else if (next_index < 0)
            {
                // –¼‘OŒŸõ
                StateBase* state = FindState(state_jugement->GetNextStateNameHash());
                if (!state) continue;
                state_jugement->SetNextStateIndex(state->GetStateIndex());
                this->next_state = state->GetStateIndex();
            }
            this->next_state = next_index;
        }
    }
}

void StateMachineComponent::PostTransitionJudgemen(StateBase* state)
{
    if (!state) return;
    for (auto& state_jugement : state->GetPostUpdateJudgementPool())
    {
        if (state->PerformTransitionJudgement(state_jugement->GetJudgement()))
        {
            StateIndex next_index = state_jugement->GetNextStateIndex();
            if (next_index == INVALID_STATE_INDEX)
            {
                // –¼‘OŒŸõ
                StateBase* state = FindState(state_jugement->GetNextStateNameHash());
                if (!state) continue;
                state_jugement->SetNextStateIndex(state->GetStateIndex());
                next_index = state->GetStateIndex();
                if (next_index >= this->state_pool.size()) continue;
                if (next_index == INVALID_STATE_INDEX) continue;
            }
            else if (next_index >= this->state_pool.size()) continue;
            this->next_state = next_index;
        }
    }
}

StateBase* StateMachineComponent::FindState(MyHash name)
{
    for (size_t i = 0; i < this->state_pool.size(); ++i)
    {
        StateBase* state = this->state_pool[i].get();
        if (state->GetHash().PerfectEqual(name))
        {
            return state;
        }
    }
    return nullptr;
}

StateIndex StateMachineComponent::FindStateIndex(MyHash name)
{
    StateBase* state = FindState(name);
    assert(state != nullptr);
    if (!state) return INVALID_STATE_INDEX;
    return state->GetStateIndex();
}

#ifdef _DEBUG

void StateMachineComponent::DrawDebugGUI()
{
    ImGui::InputSize_t("State Index", this->state_index);

    StateBase* state = this->state_pool[this->state_index].get();
    char buffer[1024];
    ::strncpy_s(buffer, sizeof(buffer), state->GetNameCStr(), sizeof(buffer));
    ImGui::InputText("State Name", buffer, sizeof(buffer));
}

#endif // DEBUG