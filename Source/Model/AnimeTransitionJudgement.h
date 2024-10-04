#pragma once
#include <memory>
class Object;

using Owner = std::shared_ptr<Object>;

// アニメーションの遷移判定を行う基底クラス
class AnimeTransitionJudgementBase
{
public:
	AnimeTransitionJudgementBase() = delete;
	AnimeTransitionJudgementBase(Owner object, bool reversal):owner_Wptr(object), should_reverse(reversal){}
	virtual ~AnimeTransitionJudgementBase() {}

	// 名前取得
	virtual const char* GetName()const = 0;;

	// 遷移判定のロジックを実行
	// `should_reverse` フラグがtrueなら、遷移判定結果を反転する
	bool PerformTransitionJudgement();

	// 各種取得・設定関数
	const bool& GetIsActive() { return this->is_active; }
	void SetIsActive(bool is_active) { this->is_active = is_active; }
protected:
	// 具体的な遷移条件の判定
	virtual bool CheckTransitionCondition() = 0;

protected:
	std::weak_ptr<Object> owner_Wptr;
private:
	bool should_reverse;	// 判定結果を反転させるかどうかのフラグ
	bool is_active = true;

#ifdef _DEBUG
public:
	virtual void DrawDebugGUI() {};
#endif // _DEBUG
};

