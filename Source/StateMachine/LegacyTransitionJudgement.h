#pragma once
#include <memory>
#include "System/Alias/OwnerAlias.h"

// アニメーションの遷移判定を行う基底クラス
class TransitionJudgementBase
{
public:
	TransitionJudgementBase() = delete;
	TransitionJudgementBase(OwnerPtr object, bool reversal ,bool require_transition_ready):owner_Wptr(object), should_reverse(reversal), require_transition_ready(require_transition_ready){}
	virtual ~TransitionJudgementBase() {}

	// 名前取得
	virtual const char* GetName()const = 0;;

	// 遷移条件の判定
	virtual bool CheckTransitionCondition() = 0;

	// 各種取得・設定関数
	bool GetIsActive() { return this->is_active; }
	bool GetShouldReversey() { return this->should_reverse; }
	bool GetRequireTransitionReady() { return this->require_transition_ready; }
	void SetIsActive(bool is_active) { this->is_active = is_active; }
	OwnerPtr GetOwner() { return this->owner_Wptr.lock(); }

protected:
	OwnerWPtr owner_Wptr;
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

