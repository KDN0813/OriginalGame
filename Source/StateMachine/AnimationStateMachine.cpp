#include "AnimationStateMachine.h"

void AnimationStateMachine::Update()
{
    this->current_state->Execute();
}

bool AnimationStateMachine::ChangeState(AnimeIndex new_state)
{
    if (this->state_pool.size() <= new_state) return false;

    this->current_state = state_pool.at(new_state);

    return false;
}

void AnimationStateMachine::RegisterState(AnimationStateBase* state)
{
    this->state_pool.emplace_back(state);
}
