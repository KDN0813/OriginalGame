#pragma once
#include "Model/AnimeTransitionJudgement.h"

class TestJudgement_Q : public AnimeTransitionJudgementBase
{
public:
	TestJudgement_Q() {}

	bool Judgement() override;
};

class TestJudgement_E : public AnimeTransitionJudgementBase
{
public:
	TestJudgement_E() {}

	bool Judgement() override;
};

