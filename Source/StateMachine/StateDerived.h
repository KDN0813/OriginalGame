#pragma once
#include <memory>
#include "State.h"

class MovementComponent;
class EnemyComponent;

class DefaultState : public StateBase
{
public:
    DefaultState() {}
    ~DefaultState() override {};

    // 開始関数
    void Start() override {};
    // 更新関数
    void Update(float elapsed_time) override {};
    // 終了関数
    void End() override {};

    const char* GetName() override { return "DefaultState"; };

    // 遷移準備が完了しているか
    // 遷移判定クラスで遷移準備を待つ設定の時に使用する
    bool IsTransitionReady() override { return true; };
#ifdef _DEBUG
public:
    void DrawDebugGUI() override{};

#endif // DEBUG
};

class IdelState : public StateBase
{
public:
    IdelState() {}
    ~IdelState() override {};

    void Start() override {};

    // 更新関数
    void Update(float elapsed_time) override {};

    const char* GetName() override { return "IdelState"; };

    // 遷移準備が完了しているか
    // 遷移判定クラスで遷移準備を待つ設定の時に使用する
    bool IsTransitionReady() override { return true; };
private:
    float state_timer = 0.0f;

#ifdef _DEBUG
public:
    void DrawDebugGUI() override {};

#endif // DEBUG
};

class WanderState : public StateBase
{
public:
    WanderState() {}
    ~WanderState() override {};

    // 開始関数
    void Start() override;
    // 個々の更新と状態遷移の更新をする
    // 更新関数
    void Update(float elapsed_time) override {};
    // 終了関数
    void End() override;

    const char* GetName() override { return "WanderState"; };

    // 遷移準備が完了しているか
    // 遷移判定クラスで遷移準備を待つ設定の時に使用する
    bool IsTransitionReady() override { return true; };
private:
    std::weak_ptr<EnemyComponent> enemy_Wptr;

#ifdef _DEBUG
public:
    void DrawDebugGUI() override {};

#endif // DEBUG
};
