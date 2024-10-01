#pragma once
#include <memory>
class Object;

using Owner = std::shared_ptr<Object>;

// アニメーションの遷移判定
class AnimeTransitionJudgementBase
{
public:
	AnimeTransitionJudgementBase() = delete;
	AnimeTransitionJudgementBase(Owner object):owner_Wptr(object){}
	virtual ~AnimeTransitionJudgementBase() {}

	virtual bool Judgement() = 0;
protected:
	std::weak_ptr<Object> owner_Wptr;
};

