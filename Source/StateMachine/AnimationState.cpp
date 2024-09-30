#include "AnimationState.h"

void AnimationStateBase::AddNextState(AnimeIndex state_id, JudgmentBase* judgment)
{
    AddNextState(StateTransitionInfo(state_id, judgment));
}

void AnimationStateBase::AddNextState(StateTransitionInfo state_transition)
{
    StateTransitions.emplace_back(state_transition);
}

void AnimationStateBase::SetOwner(AnimationStateMachine* owner)
{
    // TODO 09/30(owner‚ªnull‚È‚çƒGƒ‰[‹N‚«‚é‚æ‚¤‚É‚·‚é)
    this->owner = owner;
}
