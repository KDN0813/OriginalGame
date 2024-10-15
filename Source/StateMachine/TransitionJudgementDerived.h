#pragma once
#include <memory>
#include "StateMachine/TransitionJudgement.h"

class MovementComponent;
class ModelComponent;
class EnemyComponent;

class TestJudgement_Q : public TransitionJudgementBase
{
public:
	TestJudgement_Q(OwnerPtr object, bool reversal = false, bool require_transition_ready = false)
		:TransitionJudgementBase(object, reversal, require_transition_ready) {}

	const char* GetName()const override { return "TestJudgement_Q"; }

	bool CheckTransitionCondition() override;
};

class TestJudgement_E : public TransitionJudgementBase
{
public:
	TestJudgement_E(OwnerPtr object, bool reversal = false, bool require_transition_ready = false)
		:TransitionJudgementBase(object, reversal, require_transition_ready) {}

	const char* GetName()const override { return "TestJudgement_E"; }

	bool CheckTransitionCondition() override;
};

class Judgement_Move : public TransitionJudgementBase
{
public:
	Judgement_Move(OwnerPtr object, bool reversal = false, bool require_transition_ready = false)
		:TransitionJudgementBase(object, reversal, require_transition_ready) {}

	const char* GetName()const override { return "Move"; }

	bool CheckTransitionCondition() override;
private:
	std::weak_ptr<MovementComponent> movement_Wpt;
};

class Judgement_ButtonDown : public TransitionJudgementBase
{
	using GamePadButton = unsigned int;
public:
	Judgement_ButtonDown(OwnerPtr object, GamePadButton pad_button,bool reversal = false, bool require_transition_ready = false)
		:TransitionJudgementBase(object, reversal, require_transition_ready), pad_button(pad_button){}

	const char* GetName()const override { return "ButtonDown"; }

	bool CheckTransitionCondition() override;
private:
	GamePadButton pad_button;

#ifdef _DEBUG
public:
	void DrawDebugGUI(int unique_id) override;

#endif // _DEBUG
};

// 遷移準備が完了したら遷移する
// 遷移準備を待つフラグがオンにしてください
class Judgement_TransitionReady : public TransitionJudgementBase
{
public:
	Judgement_TransitionReady(OwnerPtr object , bool reversal = false, bool require_transition_ready = false)
		:TransitionJudgementBase(object, reversal, require_transition_ready) {}

	const char* GetName()const override { return "TransitionReady"; }

	bool CheckTransitionCondition() override { return true; };
};

// エネミーが目的地に辿り着いた時
class Judgement_IsAtTarget : public TransitionJudgementBase
{
public:
	Judgement_IsAtTarget(OwnerPtr object, bool reversal = false, bool require_transition_ready = false)
		:TransitionJudgementBase(object, reversal, require_transition_ready) {}

	const char* GetName()const override { return "Judgement_IsAtTarget"; }

	bool CheckTransitionCondition() override;
private:
	std::weak_ptr<EnemyComponent> enemy_Wptr;
};

// エネミーの待機行動が終了した時
class Judgement_IdleFinished : public TransitionJudgementBase
{
public:
	Judgement_IdleFinished(OwnerPtr object, bool reversal = false, bool require_transition_ready = false)
		:TransitionJudgementBase(object, reversal, require_transition_ready) {}

	const char* GetName()const override { return "Judgement_IsAtTarget"; }

	bool CheckTransitionCondition() override;
private:
	std::weak_ptr<EnemyComponent> enemy_Wptr;
};