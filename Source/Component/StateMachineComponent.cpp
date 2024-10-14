#include "StateMachineComponent.h"
#include "Model/AnimeTransitionJudgement.h"

StateMachineComponent::StateMachineComponent(StateIndex state_max)
{
    this->state_pool.resize(state_max);
}

void StateMachineComponent::Start()
{
}

void StateMachineComponent::End()
{
}

void StateMachineComponent::Update(float elapsed_time)
{
    //if (this->state < 0 || this->state >= this->state_pool.size()) return;

    //auto& state = this->state_pool[this->state];

    //state.PreTransitionJudgemen();
    //state.Update(elapsed_time);
    //state.PostTransitionJudgemen();
}

void StateMachineComponent::AddStateTransition(StateIndex state_index, StateIndex transition_state_index, std::unique_ptr<AnimeTransitionJudgementBase> judgement, StateBase::JudgementUpdatePhase phase)
{
    //if (this->state < 0 || this->state >= this->state_pool.size()) return;

    //this->state_pool[state_index].AddStateTransition(transition_state_index, std::move(judgement), phase);
}