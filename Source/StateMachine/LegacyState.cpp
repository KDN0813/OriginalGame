#include "LegacyState.h"
#include "StateMachine/LegacyTransitionJudgement.h"
#include "Object/Object.h"

LegacyStateTransitionInfo::LegacyStateTransitionInfo(
    std::string next_state_name,
    std::unique_ptr<TransitionJudgementBase>&& judgement
)
    :next_state_index(INVALID_STATE_INDEX),
    next_state_name(next_state_name),
    judgement(std::move(judgement))
{
}

void LegacyStateBase::SetOwner(OwnerPtr owner)
{
    this->owner_Wptr = owner;
}

bool LegacyStateBase::PerformTransitionJudgement(TransitionJudgementBase* judgemen)
{
    if (!judgemen) return false;
#ifdef _DEBUG
    if (!judgemen->GetIsActive()) return false;
#endif // _DEBUG

    // 遷移準備を待つフラグがオンの場合、遷移の準備が整うまで待機する
   if (judgemen->GetRequireTransitionReady() && !IsTransitionReady()) return false;

    return judgemen->GetShouldReversey() ? !judgemen->CheckTransitionCondition() : judgemen->CheckTransitionCondition();
}

void LegacyStateBase::AddStateTransition(std::unique_ptr<LegacyStateTransitionInfo> state_transition_info, JudgementUpdatePhase phase)
{
    auto& judgement_pool = (phase == JudgementUpdatePhase::PreUpdate) ?
        pre_update_judgement_pool : post_update_judgement_pool;
    auto& state_transition = judgement_pool.emplace_back();
    state_transition = std::move(state_transition_info);
}
