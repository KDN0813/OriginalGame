#include "State.h"
#include "Model/AnimeTransitionJudgement.h"

void StateBase::PreTransitionJudgemen()
{
    for (auto& state : this->pre_update_judgement_pool)
    {
        PerformTransitionJudgement(state.judgement.get());
    }
}

void StateBase::PostTransitionJudgemen()
{
    for (auto& state : this->post_update_judgement_pool)
    {
        PerformTransitionJudgement(state.judgement.get());
    }
}

bool StateBase::PerformTransitionJudgement(AnimeTransitionJudgementBase* judgemen)
{
    if (!judgemen) return false;
#ifdef _DEBUG
    if (!judgemen->GetIsActive()) return false;
#endif // _DEBUG

    // 遷移準備を待つフラグがオンの場合、遷移の準備が整うまで待機する
    if (judgemen->GetRequireTransitionReady()) return false;

    return judgemen->GetShouldReversey() ? !judgemen->CheckTransitionCondition() : judgemen->CheckTransitionCondition();
}

void StateBase::AddStateTransition(StateIndex state_index, std::unique_ptr<AnimeTransitionJudgementBase> judgement, JudgementUpdatePhase phase)
{
    auto& judgement_pool = (phase == JudgementUpdatePhase::PreUpdate) ?
        pre_update_judgement_pool : post_update_judgement_pool;
    auto& state_transition = judgement_pool.emplace_back();
    state_transition.state_index = state_index;
    state_transition.judgement = std::move(judgement);
}
