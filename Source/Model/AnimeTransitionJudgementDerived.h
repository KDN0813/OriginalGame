#pragma once
#include <memory>
#include "Model/AnimeTransitionJudgement.h"

class MovementComponent;
class ModelComponent;

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

class Judgement_ButtonDown : public AnimeTransitionJudgementBase
{
	using GamePadButton = unsigned int;
public:
	Judgement_ButtonDown(Owner object, GamePadButton pad_button,bool reversal = false)
		:AnimeTransitionJudgementBase(object, reversal), pad_button(pad_button){}

	bool Check() override;
private:
	GamePadButton pad_button;
};

class Judgement_AnimeEnd : public AnimeTransitionJudgementBase
{
public:
	Judgement_AnimeEnd(Owner object , bool reversal = false)
		:AnimeTransitionJudgementBase(object, reversal) {}

	bool Check() override;
private:
	std::weak_ptr<ModelComponent> model_Wptr;
};