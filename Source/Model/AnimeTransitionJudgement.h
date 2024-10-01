#pragma once

// アニメーションの遷移判定
class AnimeTransitionJudgementBase
{
public:
	AnimeTransitionJudgementBase() {}
	virtual ~AnimeTransitionJudgementBase() {}

	virtual bool Judgement() = 0;
};

