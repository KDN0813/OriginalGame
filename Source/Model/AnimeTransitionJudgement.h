#pragma once

// �A�j���[�V�����̑J�ڔ���
class AnimeTransitionJudgementBase
{
public:
	AnimeTransitionJudgementBase() {}
	virtual ~AnimeTransitionJudgementBase() {}

	virtual bool Judgement() = 0;
};

