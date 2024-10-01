#include "AnimeTransitionJudgement.h"

bool AnimeTransitionJudgementBase::Judgement()
{
    return (Check() != this->reversal);
}
