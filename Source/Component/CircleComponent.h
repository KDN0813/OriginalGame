#pragma once
#include "Component.h"
#include "Collision/CollisionParam.h"

// 円の当たり判定用コンポーネント
class CircleComponent : public Component
{
public:
    CircleComponent() {};
    virtual ~CircleComponent() {};

    // 開始関数
    void Start()  override;
    // 終了関数
    void End()  override;
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "CircleComponent"; };

    // 優先度
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::CRITICAL; };

    // 各種取得・設定関数
    CollisionType GetCollisionType() { return this->collision_type; }
    TargetType GetTargetType() { return this->target_type; }
    float GetRadius() { return this->radius; }
    void SetCollisionType(COLLISION_TYPE type) { this->collision_type = type; }
    void SetTargetType(TARGET_TYPE type) { this->target_type = type; }
    void SetRadius(float radius) { this->radius = radius; }

private:
    CollisionType collision_type = COLLISION_TYPE::COLLISION_TYPE_DEFAULT;
    TargetType target_type = TARGET_TYPE::TARGET_DEFAULT;
    float radius = 1.0f;    // 円の半径

#ifdef _DEBUG
public:
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()  override {};
    /**
     * デバックの情報を3D画面上に出力する関数
     */
    void DrawDebugPrimitive()  override {};
    /**
     * デバッグプリミティブ表示用ImGui
     */
    void DrawDebugPrimitiveGUI()  override {};
    bool IsDebugPrimitive() { return false; }   // DebugPrimitiveが存在するか
#endif // DEBUG
};

