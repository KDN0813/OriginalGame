#include <windows.h>
#include "AnimeTransitionJudgementDerived.h"

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
