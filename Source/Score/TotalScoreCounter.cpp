#include "TotalScoreCounter.h"

void TotalScoreCounter::AddScore(int value)
{
    this->score += value;

    for (const auto& on_change_score : on_change_score_pool)
    {
        if (on_change_score)
        {
            on_change_score();
        }
    }
}

void TotalScoreCounter::SetScore(int value)
{
    this->score = value;

    for (const auto& on_change_score : on_change_score_pool)
    {
        if (on_change_score)
        {
            on_change_score();
        }
    }
}

void TotalScoreCounter::Reset()
{
    this->score = 0;

    for (const auto& on_change_score : on_change_score_pool)
    {
        if (on_change_score)
        {
            on_change_score();
        }
    }
}