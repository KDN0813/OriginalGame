#include "AnimeTransitionJudgement.h"

bool AnimeTransitionJudgementBase::Judgement()
{
#ifdef _DEBUG
    if (!this->is_active) return false;
#endif // _DEBUG

    return (Check() != this->reversal);
}
