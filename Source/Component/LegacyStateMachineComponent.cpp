#include "Debug/ImGuiHelper.h"
#include "LegacyStateMachineComponent.h"
#include "StateMachine/LegacyTransitionJudgement.h"

LegacyStateMachineComponent::LegacyStateMachineComponent()
{
}

void LegacyStateMachineComponent::Start()
{
    this->state_index = (this->default_state == INVALID_STATE_INDEX) ? 0 : this->default_state;
    this->next_state = this->state_index;
    this->state_pool[this->state_index]->Start();
}

void LegacyStateMachineComponent::End()
{
}

void LegacyStateMachineComponent::ReStart()
{
    this->next_state = this->default_state;
    this->state_pool[this->state_index]->End();
    this->state_index = this->next_state;
    this->state_pool[this->state_index]->Start();
}

void LegacyStateMachineComponent::Update(float elapsed_time)
{
    if (this->state_index < 0 || this->state_index == INVALID_STATE_INDEX || this->state_index >= this->state_pool.size()) return;

    LegacyStateBase* state = this->state_pool[this->state_index].get();

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

void LegacyStateMachineComponent::ChangeState(StateIndex state_index)
{
    if (state_index < 0 || state_index == INVALID_STATE_INDEX || state_index >= this->state_pool.size()) return;
    this->next_state = state_index;
}

void LegacyStateMachineComponent::SetDefaultState(StateIndex state_index)
{
    if (state_index < 0 || state_index == INVALID_STATE_INDEX || state_index >= this->state_pool.size()) return;
    this->default_state = state_index;
}

void LegacyStateMachineComponent::SetDefaultState(std::string state_name)
{
    SetDefaultState(MyHash(state_name));
}

void LegacyStateMachineComponent::SetDefaultState(MyHash state_name)
{
    SetDefaultState(FindStateIndex(state_name));
}

void LegacyStateMachineComponent::PreTransitionJudgemen(LegacyStateBase* state)
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
                // 名前検索
                LegacyStateBase* state = FindState(state_jugement->GetNextStateNameHash());
                if (!state) continue;
                state_jugement->SetNextStateIndex(state->GetStateIndex());
                this->next_state = state->GetStateIndex();
            }
            this->next_state = next_index;
        }
    }
}

void LegacyStateMachineComponent::PostTransitionJudgemen(LegacyStateBase* state)
{
    if (!state) return;
    for (auto& state_jugement : state->GetPostUpdateJudgementPool())
    {
        if (state->PerformTransitionJudgement(state_jugement->GetJudgement()))
        {
            StateIndex next_index = state_jugement->GetNextStateIndex();
            if (next_index == INVALID_STATE_INDEX)
            {
                // 名前検索
                LegacyStateBase* state = FindState(state_jugement->GetNextStateNameHash());
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

LegacyStateBase* LegacyStateMachineComponent::FindState(MyHash name)
{
    for (size_t i = 0; i < this->state_pool.size(); ++i)
    {
        LegacyStateBase* state = this->state_pool[i].get();
        if (state->GetHash().PerfectEqual(name))
        {
            return state;
        }
    }
    return nullptr;
}

StateIndex LegacyStateMachineComponent::FindStateIndex(MyHash name)
{
    LegacyStateBase* state = FindState(name);
    assert(state != nullptr);
    if (!state) return INVALID_STATE_INDEX;
    return state->GetStateIndex();
}

#ifdef _DEBUG

void LegacyStateMachineComponent::DrawDebugGUI()
{
    ImGui::InputSize_t("State Index", this->state_index);

    int state_i = static_cast<int>(this->state_index);
    std::string play_state_name = this->state_name_pool[state_i];
    if (ImGui::ComboUI("State", play_state_name, this->state_name_pool, state_i))
    {
        this->next_state = state_i;
        this->state_pool[this->state_index]->End();
        this->state_index = this->next_state;
        this->state_pool[this->state_index]->Start();
    }
}

#endif // DEBUG