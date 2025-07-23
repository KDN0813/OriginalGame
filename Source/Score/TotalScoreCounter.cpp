#include "TotalScoreCounter.h"

void TotalScoreCounter::AddScore(int value)
{
    this->score += value;

    for (const auto& on_score_added : on_score_added_pool)
    {
        if (on_score_added)
        {
            on_score_added();
        }
    }
}

void TotalScoreCounter::SetScore(int value)
{
    this->score = value;
    for (const auto& on_score_set : on_score_set_pool)
    {
        if (on_score_set)
        {
            on_score_set();
        }
    }
}

void TotalScoreCounter::Reset()
{
    this->score = 0;
    for (const auto& on_score_set : on_score_set_pool)
    {
        if (on_score_set)
        {
            on_score_set();
        }
    }
}