#pragma once
#include <memory>
#include "LegacyState.h"

class MovementComponent;
class EnemyComponent;
class CircleCollisionComponent;

class DefaultState : public LegacyStateBase
{
public:
    DefaultState() :LegacyStateBase("DefaultState") {}
    ~DefaultState() override {};

    // 開始関数
    void Start() override {};
    // 更新関数
    void Update(float elapsed_time) override {};
    // 終了関数
    void End() override {};

    // 遷移準備が完了しているか
    // 遷移判定クラスで遷移準備を待つ設定の時に使用する
    bool IsTransitionReady() override { return true; };
#ifdef _DEBUG
public:
    void DrawDebugGUI() override{};

#endif // DEBUG
};

class IdelState : public LegacyStateBase
{
public:
    IdelState() :LegacyStateBase("IdelState") {}
    ~IdelState() override {};

    void Start() override;

    // 更新関数
    void Update(float elapsed_time) override {};

    void End()override;

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

class WanderState : public LegacyStateBase
{
public:
    WanderState() :LegacyStateBase("WanderState") {}
    ~WanderState() override {};

    // 開始関数
    void Start() override;
    // 個々の更新と状態遷移の更新をする
    // 更新関数
    void Update(float elapsed_time) override {};
    // 終了関数
    void End() override;

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

class AttackState : public LegacyStateBase
{
public:
    AttackState() :LegacyStateBase("AttackState") {}
    ~AttackState() override {};

    // 開始関数
    void Start() override;
    // 更新関数
    void Update(float elapsed_time) override;
    // 終了関数
    void End() override;

    // 遷移準備が完了しているか
    // 遷移判定クラスで遷移準備を待つ設定の時に使用する
    bool IsTransitionReady() override { return true; };
private:
    std::weak_ptr<CircleCollisionComponent> circle_collision_Wptr;

#ifdef _DEBUG
public:
    void DrawDebugGUI() override {};

#endif // DEBUG
};