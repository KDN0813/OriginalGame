#pragma once
#include <string>
#include <vector>
#include "StateMachine/Judgment.h"

using AnimeIndex = size_t; // アニメーションのインデックス

class AnimationStateMachine;

// 遷移するステート情報
struct StateTransitionInfo
{
    AnimeIndex anime_index;
    JudgmentBase* judgment;
};

// アニメーション状態
class AnimationStateBase
{
public:
    AnimationStateBase(){}
    ~AnimationStateBase(){}

    // 実行
    virtual void Execute() = 0;

    // 遷移するステートの追加
    void AddNextState(AnimeIndex state_id, JudgmentBase* judgment);
    void AddNextState(StateTransitionInfo state_transition);

    // 各設定・取得関数
    void SetOwner(AnimationStateMachine* owner);
private:
    std::string name;
    std::vector<StateTransitionInfo> StateTransitions;

    AnimationStateMachine* owner = nullptr;
};

