#pragma once
#include <memory>
#include "Model/AnimeTransitionJudgement.h"

class MovementComponent;
class ModelComponent;

class TestJudgement_Q : public AnimeTransitionJudgementBase
{
public:
	TestJudgement_Q(OwnerPtr object, bool reversal = false, bool require_transition_ready = false)
		:AnimeTransitionJudgementBase(object, reversal, require_transition_ready) {}

	const char* GetName()const override { return "TestJudgement_Q"; }

	bool CheckTransitionCondition() override;
};

class TestJudgement_E : public AnimeTransitionJudgementBase
{
public:
	TestJudgement_E(OwnerPtr object, bool reversal = false, bool require_transition_ready = false)
		:AnimeTransitionJudgementBase(object, reversal, require_transition_ready) {}

	const char* GetName()const override { return "TestJudgement_E"; }

	bool CheckTransitionCondition() override;
};

class Judgement_Move : public AnimeTransitionJudgementBase
{
public:
	Judgement_Move(OwnerPtr object, bool reversal = false, bool require_transition_ready = false)
		:AnimeTransitionJudgementBase(object, reversal, require_transition_ready) {}

	const char* GetName()const override { return "Move"; }

	bool CheckTransitionCondition() override;
private:
	std::weak_ptr<MovementComponent> movement_Wpt;
};

class Judgement_ButtonDown : public AnimeTransitionJudgementBase
{
	using GamePadButton = unsigned int;
public:
	Judgement_ButtonDown(OwnerPtr object, GamePadButton pad_button,bool reversal = false, bool require_transition_ready = false)
		:AnimeTransitionJudgementBase(object, reversal, require_transition_ready), pad_button(pad_button){}

	const char* GetName()const override { return "ButtonDown"; }

	bool CheckTransitionCondition() override;
private:
	GamePadButton pad_button;

#ifdef _DEBUG
public:
	void DrawDebugGUI(int unique_id) override;

#endif // _DEBUG
};

// ‘JˆÚ€”õ‚ªŠ®—¹‚µ‚½‚ç‘JˆÚ‚·‚é
// ‘JˆÚ€”õ‚ğ‘Ò‚Âƒtƒ‰ƒO‚ªƒIƒ“‚É‚µ‚Ä‚­‚¾‚³‚¢
class Judgement_TransitionReady : public AnimeTransitionJudgementBase
{
public:
	Judgement_TransitionReady(OwnerPtr object , bool reversal = false, bool require_transition_ready = false)
		:AnimeTransitionJudgementBase(object, reversal, require_transition_ready) {}

	const char* GetName()const override { return "TransitionReady"; }

	bool CheckTransitionCondition() override { return true; };
};