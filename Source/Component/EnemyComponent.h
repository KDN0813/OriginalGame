#pragma once
#include "Component.h"
#include "DirectXMath.h"

class Transform3DComponent;
class MovementComponent;
class CircleCollisionComponent;
class CharacterComponent;

class EnemyComponent : public Component
{
public:
    struct EnemyParam
    {
        DirectX::XMFLOAT3 target_position{};
        float territory_range = 45.0f;
        float radius = 1.0f;
        float move_speed = 3.0f;
        float speed_rate = 0.5f;

        float idle_timer = 0.0f;    // 待機時間
        float max_idle_time = 5.0f;
        float min_idle_time = 0.5f;
        bool move_validity_flag = true;   // 移動が有効であるかのフラグ
    };
public:
    EnemyComponent(EnemyParam param) :param(param),default_param(param){};
    ~EnemyComponent() {};

    // 開始関数
    void Start() override;
    // 終了関数
    void End() override;
    // リスタート処理
    void ReStart() override { this->param = this->default_param; };      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "EnemyComponent"; };

    // 優先度
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::VERY_HIGH; };

    // 次の目的地設定
    void SetRandomTargetPosition();
    // 待機時間設定
    void SetRandomIdleTime();
    // 目的地に到達しているか
    bool IsAtTarget();
    bool IsAtTarget(float distSq);
    // 現在待機行動中であるか
    bool IsIdle() { return (this->param.idle_timer > 0.0f); }

    // 各種・設定取得関数
    bool GetMoveValidityFlag() const { return this->param.move_validity_flag; }
    float GetIdleTime() { return this->param.idle_timer; }
    void SetIdleTime(float time) { this->param.idle_timer = time; }
    void SetMoveValidityFlag(bool flag) { this->param.move_validity_flag = flag; }
private:
    void Move(float vx, float vz, float speed);
    void MoveToTarget(float elapsed_time, std::shared_ptr<Transform3DComponent>& transform, float speed_rate);

private:
    EnemyParam param;
    EnemyParam default_param;
private:
    std::weak_ptr<MovementComponent> movement_Wptr;
    std::weak_ptr<Transform3DComponent> transform_Wptr;
    std::weak_ptr<CircleCollisionComponent> circle_collision_Wptr;
    std::weak_ptr<CharacterComponent> character_Wptr;
#ifdef _DEBUG
public:
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI() override;
    /**
     * デバックの情報を3D画面上に出力する関数
     */
    void DrawDebugPrimitive() override;
#endif // DEBUG
};

