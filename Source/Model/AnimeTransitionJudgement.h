#pragma once
#include <memory>
class Object;

using Owner = std::shared_ptr<Object>;

// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ì‘JˆÚ”»’è
class AnimeTransitionJudgementBase
{
public:
	AnimeTransitionJudgementBase() = delete;
	AnimeTransitionJudgementBase(Owner object, bool reversal):owner_Wptr(object), reversal(reversal){}
	virtual ~AnimeTransitionJudgementBase() {}

	// reversalƒtƒ‰ƒO‚ªTrue‚È‚çCheck‚Ì³Œë”»’è‚ª‹t“]‚·‚é
	// ‘JˆÚ”»’è
	bool Judgement();
protected:
	// ’Êí‚Ì‘JˆÚ”»’è‚ğ‚·‚é
	virtual bool Check() = 0;

protected:
	std::weak_ptr<Object> owner_Wptr;
private:
	bool reversal;	// ”»’èŒ‹‰Ê‚ğ”½“]‚³‚¹‚éƒtƒ‰ƒO
};

