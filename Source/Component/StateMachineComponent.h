#pragma once
#include "Component.h"
#include "StateMachine/State.h"

class StateMachineComponent : public Component
{
public:
    StateMachineComponent() {};
    virtual ~StateMachineComponent() {};

    // 開始関数
    void Start()  override;
    // 終了関数
    void End()  override;
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "StateMachineComponent"; };

    // 優先度
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::CRITICAL; };

private:
    std::vector<StateBase> state_pool;

#ifdef _DEBUG
public:
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()  override {};
#endif // DEBUG
};

