#include <windows.h>
#include "AnimeTransitionJudgementDerived.h"

#include "Object/Object.h"

#include "Component/MovementComponent.h"

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
