#pragma once
#include "StateMachine/AnimationState.h"

class AnimationStateMachine
{
public:
    AnimationStateMachine(){}
    ~AnimationStateMachine(){}

	// 更新処理
	void Update();
	// ステート変更
	bool ChangeState(AnimeIndex new_state);
	// ステート登録
	void RegisterState(AnimationStateBase* state);
private:
	AnimationStateBase* current_state = nullptr; // 現在のステート
    std::vector<AnimationStateBase*> state_pool;
};