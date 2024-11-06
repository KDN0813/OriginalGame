#pragma once
#include <DirectXMath.h>
#include "Component.h"

class MovementComponent;
class Transform3DComponent;
class CircleCollisionComponent;
class CharacterComponent;

class PlayerComponent : public Component
{
public:
    struct PlayerParam
    {
        float move_speed = 10.0f;
        int damage_amount = 1;
        bool input_move_validity_flag = true;   // 入力による移動が有効であるかのフラグ
    };
public:
    PlayerComponent(PlayerParam param) :param(param), default_param(param) {};
    ~PlayerComponent();
    // リスタート処理
    void ReStart() override { this->param = this->default_param; };      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override;
    // 名前取得
    const char* GetName()const override { return "PlayerComponent"; };
    const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::VERY_HIGH; };

    bool GetInputMoveValidityFlag()const { return this->param.input_move_validity_flag; }
    void SetInputMoveValidityFlag(bool flag) { this->param.input_move_validity_flag = flag; }

    // 他オブジェクトに接触した時の処理
    void OnCollision(const std::shared_ptr<Object>& hit_object)override;
private:
    bool InputMove(float elapsed_time);
    void Move(float vx, float vz, float speed);
    DirectX::XMFLOAT3 GetMoveVec() const;

private:
    PlayerParam param;
    PlayerParam default_param;

private:
    std::weak_ptr<MovementComponent> movement_Wptr;
    std::weak_ptr<Transform3DComponent> transform_Wptr;

#ifdef _DEBUG
public:
    void DrawDebugGUI() override;
#endif // _DEBUG

#ifdef RELEASE_DEBUG
public:
    void DebugCheat()override;
#endif
};

