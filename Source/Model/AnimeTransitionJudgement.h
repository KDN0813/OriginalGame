#pragma once
#include <memory>
class Object;

using Owner = std::shared_ptr<Object>;

// アニメーションの遷移判定
class AnimeTransitionJudgementBase
{
public:
	AnimeTransitionJudgementBase() = delete;
	AnimeTransitionJudgementBase(Owner object, bool reversal):owner_Wptr(object), reversal(reversal){}
	virtual ~AnimeTransitionJudgementBase() {}

	// 名前取得
	virtual const char* GetName()const = 0;;

	// reversalフラグがTrueならCheckの正誤判定が逆転する
	// 遷移判定
	bool Judgement();
protected:
	// 通常時の遷移判定をする
	virtual bool Check() = 0;

protected:
	std::weak_ptr<Object> owner_Wptr;
private:
	bool reversal;	// 判定結果を反転させるフラグ

#ifdef _DEBUG
public:
	virtual void DrawDebugGUI() {};
#endif // _DEBUG
};

