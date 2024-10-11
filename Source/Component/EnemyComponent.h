#pragma once
#include "Component.h"

class EnemyComponent : public Component
{
public:
    EnemyComponent() {};
    ~EnemyComponent() {};

    // 開始関数
    void Start() override;
    // 終了関数
    void End() override;
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "EnemyComponent"; };

    // 優先度
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::CRITICAL; };

#ifdef _DEBUG
public:
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI() override;
#endif // DEBUG
};

