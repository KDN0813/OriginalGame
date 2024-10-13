#include "State.h"

void StateBase::AddStateTransition(StateIndex state_index, std::unique_ptr<AnimeTransitionJudgementBase> judgement, JudgementUpdatePhase phase)
{
    auto& judgement_pool = (phase == JudgementUpdatePhase::PreUpdate) ?
        pre_update_judgement_pool : post_update_judgement_pool;
    auto& state_transition = judgement_pool.emplace_back();
    state_transition.state_index = state_index;
    state_transition.judgement = std::move(judgement);
}
