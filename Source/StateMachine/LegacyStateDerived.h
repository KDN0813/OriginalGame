#pragma once
#include <memory>
#include "LegacyState.h"

class MovementComponent;
class EnemyComponent;
class CircleCollisionComponent;
class InstancedModelWithStateAnimationComponent;

class Legacy_DefaultState : public LegacyStateBase
{
public:
    Legacy_DefaultState() :LegacyStateBase("DefaultState") {}
    ~Legacy_DefaultState() override {};

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

class Legacy_IdelState : public LegacyStateBase
{
public:
    Legacy_IdelState() :LegacyStateBase("IdelState") {}
    ~Legacy_IdelState() override {};

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

class Legacy_WanderState : public LegacyStateBase
{
public:
    Legacy_WanderState() :LegacyStateBase("WanderState") {}
    ~Legacy_WanderState() override {};

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

class Legacy_DamageState : public LegacyStateBase
{
public:
    Legacy_DamageState() :LegacyStateBase("DamageState") {}
    ~Legacy_DamageState() override {};

    // 開始関数
    void Start() override;
    // 個々の更新と状態遷移の更新をする
    // 更新関数
    void Update(float elapsed_time) override;
    // 終了関数
    void End() override;

    // 遷移準備が完了しているか
    // 遷移判定クラスで遷移準備を待つ設定の時に使用する
    bool IsTransitionReady() override;
private:
    std::weak_ptr<EnemyComponent> enemy_Wptr;
    std::weak_ptr<InstancedModelWithStateAnimationComponent> model_Wptr;

#ifdef _DEBUG
public:
    void DrawDebugGUI() override {};

#endif // DEBUG
};

class Legacy_DeathIdleState : public LegacyStateBase
{
public:
    Legacy_DeathIdleState() :LegacyStateBase("DeathIdleState") {}
    ~Legacy_DeathIdleState() override {};

    // 開始関数
    void Start() override {};
    // 個々の更新と状態遷移の更新をする
    // 更新関数
    void Update(float elapsed_time) override {};
    // 終了関数
    void End() override {};

private:
    std::weak_ptr<EnemyComponent> enemy_Wptr;
    std::weak_ptr<InstancedModelWithStateAnimationComponent> model_Wptr;

#ifdef _DEBUG
public:
    void DrawDebugGUI() override {};

#endif // DEBUG
};

class Legacy_AttackState : public LegacyStateBase
{
public:
    Legacy_AttackState() :LegacyStateBase("AttackState") {}
    ~Legacy_AttackState() override {};

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