#pragma once
#include <DirectXMath.h>
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

#include "Component.h"

class MovementComponent;
class Transform3DComponent;
class CircleCollisionComponent;
class CharacterComponent;
class CircleCollisionComponent;

class PlayerComponent : public Component
{
public:
    struct PlayerParam
    {
        float move_speed = 10.0f;
        int damage_amount = 1;
        bool input_move_validity_flag = true;   // 入力による移動が有効であるかのフラグ
    
        float move_rate = 1.0f; // 速度倍率
        float spin_attack_move_rate = 2.0f;

        int spin_attack_use_gage_count = 1;     // 回転攻撃に必要なゲージ数
        bool is_spin_attack = false;            // 回転攻撃中であるか
        float spin_attack_time = 3.0f;          // 回転攻撃を行う時間
        float spin_attack_timer = 0.0f;         // 回転攻撃を行う残り時間

        float old_special_point = 0.0f;          // 前回のスペシャルポイント
        float special_point = 0.0f;          // スペシャルポイント
        float special_point_max = 20.0f;    // スペシャルポイントの最大値
        int gauge_count = 0;
        int gauge_count_max = 3;

        Effekseer::Handle spin_attack_effect_handle = -1;
    };
public:
    PlayerComponent(PlayerParam param);
    ~PlayerComponent();
    // リスタート処理
    void ReStart() override { this->param = this->default_param; };      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override;
    // 名前取得
    const char* GetName()const override { return "PlayerComponent"; };
    const PRIORITY GetPriority()const noexcept override { return PRIORITY::VERY_HIGH; };

    bool GetInputMoveValidityFlag()const { return this->param.input_move_validity_flag; }
    void SetInputMoveValidityFlag(bool flag) { this->param.input_move_validity_flag = flag; }

    // 他オブジェクトに接触した時の処理
    void OnCollision(const std::shared_ptr<Object>& hit_object)override;

    const float GetMoveRate()const { return this->param.move_rate; }
    const float GetSpinAttackMoveRate()const { return this->param.spin_attack_move_rate; }
    void SetMoveRate(const float move_rate) { this->param.move_rate = move_rate; }

    const float GetSpinAttackTime()const { return this->param.spin_attack_time; }

    const int GetGageCount()const { return this->param.gauge_count; }
    const int GetGageCountMax()const { return this->param.gauge_count_max; }

    const float GetSpinAttackTimer() const { return this->param.spin_attack_timer; }
    void SetSpinAttackTimer(float time) { this->param.spin_attack_timer = time; }

    void AddSpecialPoint(float point);
    void SetSpecialPoint(float point) { this->param.special_point = (std::min)(point, this->param.special_point_max); }
    const float GetSpecialPoint()const { return this->param.special_point; }
    const float GetSpecialPointMax()const { return this->param.special_point_max; }
    const bool GetIsSpinAttack()const { return this->param.is_spin_attack; }
    void SetIsSpinAttack(bool flag) { this->param.is_spin_attack = flag; }

    // スペシャルポイントが上昇したか
    bool IsAddSpecialPoint();

    const int GetSpinAttackUseGageCount()const { return this->param.spin_attack_use_gage_count; }
    // スペシャルゲージを使用する
    // 使用できたらTrueを返す
    bool UseSpecialGage(int use_gage);
    // スペシャルゲージを使用できるか判定する
    bool IsUseSpecialGage(int use_gage);

    void SetSpinAttackEffectHandle(Effekseer::Handle handle) { this->param.spin_attack_effect_handle = handle; }
    Effekseer::Handle GetSpinAttackEffectHandle()const { return this->param.spin_attack_effect_handle; }
private:
    bool InputMove(float elapsed_time);
    void Move(float vx, float vz, float speed);
    DirectX::XMFLOAT3 GetMoveVec() const;

private:
    PlayerParam param;
    PlayerParam default_param;

public:
    // HACK 変数名・変数の位置変更する
    // この時間を超えると他ステートに遷移可能になる
    float attack_end_point = 0.2f;
    float attack_combo2_end_point = 0.25f;
private:
    std::weak_ptr<MovementComponent> movement_Wptr;
    std::weak_ptr<Transform3DComponent> transform_Wptr;
    std::weak_ptr<CharacterComponent> character_Wptr;
    std::weak_ptr<CircleCollisionComponent> circle_collision_Wptr;

#ifdef _DEBUG
public:
    void DrawDebugGUI() override;
    bool IsDebugPrimitive() override  { return false; }   // DebugPrimitiveが存在するか

    static float debug_move_speed;    // Debug時の移動速度
#endif // _DEBUG

#ifdef RELEASE_DEBUG
public:
    void DebugCheat()override {};
#endif
};

