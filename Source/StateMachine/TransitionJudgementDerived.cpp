#include <windows.h>
#include <imgui.h>
#include "TransitionJudgementDerived.h"

#include "Input/Input.h"
#include "Object/Object.h"

#include "Component/MovementComponent.h"
#include "Component/EnemyComponent.h"

bool TestJudgement_Q::CheckTransitionCondition()
{
	if (GetAsyncKeyState('Q') & 0x8000)
	{
		return true;
	}

	return false;
}

bool TestJudgement_E::CheckTransitionCondition()
{
	if (GetAsyncKeyState('E') & 0x8000)
	{
		return true;
	}


	return false;
}

bool Judgement_Move::CheckTransitionCondition()
{
	auto owner = this->owner_Wptr.lock();
	if (!owner) return false;
	auto movement = owner->EnsureComponentValid(this->movement_Wpt);
	if (!movement) return false;
	if (!movement->IsMoveXZAxis()) return false;

	return true;
}

bool Judgement_ButtonDown::CheckTransitionCondition()
{
	GamePad& pad = Input::Instance()->GetGamePad();
	return (pad.GetButtonDown() & GamePad::BTN_X);
}

#ifdef _DEBUG

void Judgement_ButtonDown::DrawDebugGUI(int unique_id)
{
	ImGui::Text("test_�����Ă�H");
}

#endif // _DEBUG

bool Judgement_IsAtTarget::CheckTransitionCondition()
{
	auto owner = this->owner_Wptr.lock();
	if (!owner) return false;
	auto enemy = owner->EnsureComponentValid(this->enemy_Wptr);
	if (!enemy) return false;
	return enemy->IsAtTarget();
}

bool Judgement_IdleFinished::CheckTransitionCondition()
{
	auto owner = this->owner_Wptr.lock();
	if (!owner) return false;
	auto enemy = owner->EnsureComponentValid(this->enemy_Wptr);
	if (!enemy) return false;
	return !enemy->IsIdle();
}