#pragma once
#include <memory>
#include "Model/AnimeTransitionJudgement.h"

class MovementComponent;

class TestJudgement_Q : public AnimeTransitionJudgementBase
{
public:
	TestJudgement_Q(Owner object)
		:AnimeTransitionJudgementBase(object) {}

	bool Judgement() override;
};

class TestJudgement_E : public AnimeTransitionJudgementBase
{
public:
	TestJudgement_E(Owner object)
		:AnimeTransitionJudgementBase(object) {}

	bool Judgement() override;
};

class Judgement_Move : public AnimeTransitionJudgementBase
{
public:
	Judgement_Move(Owner object)
		:AnimeTransitionJudgementBase(object) {}

	bool Judgement() override;
private:
	std::weak_ptr<MovementComponent> movement_Wpt;
};