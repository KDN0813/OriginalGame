#pragma once
#include <vector>
#include "Component.h"
#include "Collision/CollisionParam.h"
#ifdef _DEBUG
#include "Debug/DebugPrimitiveRenderer.h"
#endif // _DEBUG

class Transform3DComponent;

// 円の当たり判定用コンポーネント
class CircleCollisionComponent : public Component , public std::enable_shared_from_this<CircleCollisionComponent>
{
public:
    struct CollisionParam
    {
        COLLISION_OBJECT_TYPE collision_type = COLLISION_OBJECT_TYPE::NONE;
        float radius = 1.0f;                // 円の半径
        bool hit_flag = false;    
        bool old_hit_flag = false; 
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
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::LOWEST; };

    // 接触判定を行う
    void EvaluateCollision();

    // 他オブジェクトに接触した時の処理
    void OnCollision(const std::shared_ptr<Object>& hit_object)override;

    // 接触した瞬間処理するコンポーネントの追加
    void AddCollisionComponent(const std::shared_ptr<Component> component);

    // 各種取得・設定関数
    COLLISION_OBJECT_TYPE GetCollisionType() const { return this->param.collision_type; }
    float GetRadius() const { return this->param.radius; }
    CircleParam GetCircleParam();
    CircleHitResult GetCircleHitResult()const { return this->hit_result; }
    void SetCollisionType(COLLISION_OBJECT_TYPE type) { this->param.collision_type = type; }
    void SetRadius(float radius) { this->param.radius = radius; }
    void SetHitResult(CircleHitResult result) { this->hit_result = result; }
    void SetHitFlag(bool flag) { this->param.hit_flag = flag; }
private:

private:
    CollisionParam param;
    CollisionParam default_param;
    CircleHitResult hit_result{};

    std::vector<std::weak_ptr<Component>> collision_component_Wptr_pool;    // 接触した瞬間処理するコンポーネント
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

