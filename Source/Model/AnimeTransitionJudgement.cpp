#include "AnimeTransitionJudgement.h"

bool AnimeTransitionJudgementBase::PerformTransitionJudgement()
{
#ifdef _DEBUG
    if (!this->is_active) return false;
#endif // _DEBUG

    return should_reverse ? !CheckTransitionCondition() : CheckTransitionCondition();
}
