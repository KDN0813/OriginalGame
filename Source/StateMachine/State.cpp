#include "State.h"
#include "StateMachine/TransitionJudgement.h"
#include "Object/Object.h"

StateTransitionInfo::StateTransitionInfo(
    std::string next_state_name,
    std::unique_ptr<TransitionJudgementBase> judgement
)
    :next_state_index(-1),
    next_state_name(next_state_name),
    judgement(std::move(judgement))
{
}

void StateBase::SetOwner(OwnerPtr owner)
{
    this->owner_Wptr = owner;
}

bool StateBase::PerformTransitionJudgement(TransitionJudgementBase* judgemen)
{
    if (!judgemen) return false;
#ifdef _DEBUG
    if (!judgemen->GetIsActive()) return false;
#endif // _DEBUG

    // 遷移準備を待つフラグがオンの場合、遷移の準備が整うまで待機する
    if (judgemen->GetRequireTransitionReady() && !IsTransitionReady()) return false;

    return judgemen->GetShouldReversey() ? !judgemen->CheckTransitionCondition() : judgemen->CheckTransitionCondition();
}

void StateBase::AddStateTransition(std::unique_ptr<StateTransitionInfo> state_transition_info, JudgementUpdatePhase phase)
{
    auto& judgement_pool = (phase == JudgementUpdatePhase::PreUpdate) ?
        pre_update_judgement_pool : post_update_judgement_pool;
    auto& state_transition = judgement_pool.emplace_back();
    state_transition = std::move(state_transition_info);
}
