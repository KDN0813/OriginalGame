#include <windows.h>
#include "AnimeTransitionJudgementDerived.h"

#include "Input/Input.h"
#include "Object/Object.h"

#include "Component/MovementComponent.h"
#include "Component/ModelComponent.h"

bool TestJudgement_Q::Check()
{
	if (GetAsyncKeyState('Q') & 0x8000)
	{
		return true;
	}

	return false;
}

bool TestJudgement_E::Check()
{
	if (GetAsyncKeyState('E') & 0x8000)
	{
		return true;
	}


	return false;
}

bool Judgement_Move::Check()
{
	auto owner = this->owner_Wptr.lock();
	if (!owner) return false;
	auto movement = owner->GetComponent(this->movement_Wpt);
	if (!movement) return false;
	if (!movement->IsMoveXZAxis()) return false;

	return true;
}

bool Judgement_ButtonDown::Check()
{
	GamePad& pad = Input::Instance()->GetGamePad();
	return (pad.GetButtonDown() & GamePad::BTN_X);
}

bool Judgement_AnimeEnd::Check()
{
	auto owner = this->owner_Wptr.lock();
	if (!owner) return false;
	auto model = owner->GetComponent(this->model_Wptr);
	if (!model) return false;
	if (model->IsPlayAnimation()) return false;
	
	return true;
}
