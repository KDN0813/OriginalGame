#pragma once
#include "Component.h"
#include "Collision/CollisionParam.h"
#ifdef _DEBUG
#include "Debug/DebugPrimitiveRenderer.h"
#endif // _DEBUG

class Transform3DComponent;

// 円の当たり判定用コンポーネント
class CircleCollisionComponent : public Component
{
public:
    struct CollisionParam
    {
        COLLISION_TYPE collision_type = COLLISION_TYPE::NONE;
        float radius = 1.0f;    // 円の半径
        bool default_active_flag = true;    // アクティブフラグの初期値
    };
public:
    CircleCollisionComponent(CollisionParam param);
    ~CircleCollisionComponent() {};

    // 開始関数
    void Start()  override;
    // 終了関数
    void End()  override;
    // リスタート処理
    void ReStart() override;      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "CircleCollisionComponent"; };

    // 優先度
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::CRITICAL; };

    // 各種取得・設定関数
    COLLISION_TYPE GetCollisionType() const { return this->param.collision_type; }
    float GetRadius() const { return this->param.radius; }
    CircleParam GetCircleParam();
    bool GetHitFlag() const { return this->hit_flag; }
    void SetCollisionType(COLLISION_TYPE type) { this->param.collision_type = type; }
    void SetRadius(float radius) { this->param.radius = radius; }
    void SetHitFlag(bool hit) { this->hit_flag = hit; }
    void SetHitResult(CircleHitResult result) { this->hit_result = result; }

private:
    CollisionParam param;
    CollisionParam default_param;
    bool hit_flag = false;
    CircleHitResult hit_result{};
private:
    std::weak_ptr<Transform3DComponent> transform_Wptr;
#ifdef _DEBUG
public:
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()  override;
    /**
     * デバックの情報を3D画面上に出力する関数
     */
    void DrawDebugPrimitive()  override;
    /**
     * デバッグプリミティブ表示用ImGui
     */
    void DrawDebugPrimitiveGUI()  override;
    bool IsDebugPrimitive() { return true; }   // DebugPrimitiveが存在するか
    CylinderParam circle_collsion_primitive;
#endif // DEBUG
private:
    float height = 1.3f;    // circle_collsion_primitiveの円柱の高さ
};

