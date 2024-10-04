#pragma once
#include <memory>
class Object;
class ModelComponent;

using Owner = Object;
using OwnerPtr = std::shared_ptr<Owner>;

// アニメーションの遷移判定を行う基底クラス
class AnimeTransitionJudgementBase
{
public:
	AnimeTransitionJudgementBase() = delete;
	AnimeTransitionJudgementBase(OwnerPtr object, bool reversal ,bool require_transition_ready):owner_Wptr(object), should_reverse(reversal), require_transition_ready(require_transition_ready){}
	virtual ~AnimeTransitionJudgementBase() {}

	// 名前取得
	virtual const char* GetName()const = 0;;

	// 遷移判定のロジックを実行
	// `should_reverse` フラグがtrueなら、遷移判定結果を反転する
	bool PerformTransitionJudgement();

	// 各種取得・設定関数
	const bool& GetIsActive() { return this->is_active; }
	void SetIsActive(bool is_active) { this->is_active = is_active; }
	OwnerPtr GetOwner() { return this->owner_Wptr.lock(); }
protected:
	// 具体的な遷移条件の判定
	virtual bool CheckTransitionCondition() = 0;

protected:
	std::weak_ptr<Owner> owner_Wptr;
	std::weak_ptr<ModelComponent> model_Wptr;
private:
	bool should_reverse;			// 判定結果を反転させるかどうかのフラグ
	bool require_transition_ready;	// 遷移の準備が整っていることが必要かどうかを示すフラグ
	bool is_active = true;

#ifdef _DEBUG
public:
	void DrawCommonDebugGUI(int unique_id);	// 共通のDebugGUI
	virtual void DrawDebugGUI(int unique_id) {};
#endif // _DEBUG
};

