#pragma once
#include "Component.h"
#include "DirectXMath.h"

class Transform3DComponent;
class MovementComponent;

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

    void Move(float vx, float vz, float speed);
    void Turn(float elapsed_time, float vx, float vz, float speed);
    void MoveToTarget(float elapsed_time, std::shared_ptr<Transform3DComponent>& transform,float speed_rate);
    void SetRandomTargetPosition();
private:
    DirectX::XMFLOAT3 target_position{};
    DirectX::XMFLOAT3 territory_origin{};
    float territory_range = 30.0f;
    float radius = 5.0f;
    float move_speed = 3.0f;
    float speed_rate = 0.5f;
    float turn_speed = DirectX::XMConvertToRadians(360);

private:
    std::weak_ptr<MovementComponent> movement_Wptr;
    std::weak_ptr<Transform3DComponent> transform_Wptr;
#ifdef _DEBUG
public:
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI() override;
#endif // DEBUG
};

