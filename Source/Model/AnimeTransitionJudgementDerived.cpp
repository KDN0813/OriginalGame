#include <windows.h>
#include "AnimeTransitionJudgementDerived.h"

#include "Object/Object.h"

#include "Component/MovementComponent.h"

bool TestJudgement_Q::Judgement()
{
	if (GetAsyncKeyState('Q') & 0x8000)
	{
		return true;
	}

	return false;
}

bool TestJudgement_E::Judgement()
{
	if (GetAsyncKeyState('E') & 0x8000)
	{
		return true;
	}


	return false;
}

bool Judgement_Move::Judgement()
{
	auto owner = this->owner_Wptr.lock();
	if (!owner) return false;
	auto movement = owner->GetComponent(this->movement_Wpt);
	if (!movement) return false;
	if (!movement->IsMoveXZAxis()) return false;

	return true;
}
