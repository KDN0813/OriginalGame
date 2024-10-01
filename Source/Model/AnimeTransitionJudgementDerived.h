#pragma once
#include <memory>
#include "Model/AnimeTransitionJudgement.h"

class MovementComponent;

class TestJudgement_Q : public AnimeTransitionJudgementBase
{
public:
	TestJudgement_Q(Owner object, bool reversal = false)
		:AnimeTransitionJudgementBase(object, reversal) {}

	bool Check() override;
};

class TestJudgement_E : public AnimeTransitionJudgementBase
{
public:
	TestJudgement_E(Owner object, bool reversal = false)
		:AnimeTransitionJudgementBase(object, reversal) {}

	bool Check() override;
};

class Judgement_Move : public AnimeTransitionJudgementBase
{
public:
	Judgement_Move(Owner object, bool reversal = false)
		:AnimeTransitionJudgementBase(object, reversal) {}

	bool Check() override;
private:
	std::weak_ptr<MovementComponent> movement_Wpt;
};