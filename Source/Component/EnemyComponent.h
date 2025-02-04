#pragma once
#include "Component.h"
#include "DirectXMath.h"
#include "Object\Constant\EnemyConstant.h"

class Transform3DComponent;
class MovementComponent;
class CircleCollisionComponent;
class CharacterComponent;
class InstancedModelWithAnimationComponent;

class EnemyComponent : public Component
{
public:
    enum class STATE
    {
        IDLE = 0,   // 待機
        WANDERING,  // 放浪
        CHASE,      // 接近
        ATTACK,     // 攻撃
        DAMAGE,     // ダメージ
        DETH,       // 死亡
    };

    struct EnemyParam
    {
        DirectX::XMFLOAT3 target_position{};
        float territory_range = EnemyConstant::DEFAULT_TERRITORY_RENGR;
        float radius = 0.1f;
        float move_speed = 5.0f;
        float speed_rate = 0.5f;

        float attack_range = 3.0f;    // 攻撃範囲

        float idle_timer = 0.0f;    // 待機時間
        float max_idle_time = 5.0f;
        float min_idle_time = 0.5f;
        float remove_timer = 2.0f;    // 削除タイマー
        float damage_effect_offset_up = 0.5f;   // ダメージエフェクトを受ける時の上方向のoffset値
        int damage_amount = 1;
        bool move_validity_flag = true;   // 移動が有効であるかのフラグ
        bool pending_removal_flag = false;     // 削除待ちをしているか(死亡演出を待っているか)
        STATE state = STATE::IDLE;  // 状態
        DirectX::XMFLOAT3 spawn_point{};    // スポーン位置
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
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::VERY_HIGH; };

    // 他オブジェクトに接触した時の処理
    void OnCollision(const std::shared_ptr<Object>& hit_object) override;

    // 次の目的地設定
    void SetRandomTargetPosition();
    // 次の目的地をプレイヤーの位置に設定
    void SetTargetPositionByPlayer();
    // 待機時間設定
    void SetRandomIdleTime();
    // 目的地に到達しているか
    bool IsAtTarget();
    bool IsAtTarget(float distSq);
    // 移動範囲内にプレイヤーが存在するか
    bool IsPlayerInMovementArea();
    // 攻撃範囲内にプレイヤーが存在するか
    bool IsPlayerInAttacklArea();

    // 現在待機行動中であるか
    bool IsIdle() { return (this->param.idle_timer > 0.0f); }

    // 各種・設定取得関数
    bool GetMoveValidityFlag() const { return this->param.move_validity_flag; }
    float GetIdleTime() { return this->param.idle_timer; }
    void SetIdleTime(float time) { this->param.idle_timer = time; }
    void SetMoveValidityFlag(bool flag) { this->param.move_validity_flag = flag; }
    void SetPendingRemovalFlag(bool flag) { this->param.pending_removal_flag = flag; }
private:
    void Move(float vx, float vz, float speed);
    void MoveToTarget(float elapsed_time, std::shared_ptr<Transform3DComponent>& transform, float speed_rate);

    // ダメージステートに遷移
    void SetDamageState();
private:
    EnemyParam param;
    EnemyParam default_param;
private:
    std::weak_ptr<MovementComponent> movement_Wptr;
    std::weak_ptr<Transform3DComponent> transform_Wptr;
    std::weak_ptr<CircleCollisionComponent> circle_collision_Wptr;
    std::weak_ptr<Transform3DComponent> child_transform_Wptr;
    std::weak_ptr<CharacterComponent> character_Wptr;
    std::weak_ptr<InstancedModelWithAnimationComponent> model_Wptr;
    std::weak_ptr<CircleCollisionComponent> child_collision_Wprt;   // 攻撃用のコリジョン

    std::weak_ptr<Transform3DComponent> player_transform_Wptr;
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

