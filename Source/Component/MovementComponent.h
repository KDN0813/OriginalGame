#pragma once
#include <memory>
#include <DirectXMath.h>
#include "Component.h"

class Transform3DComponent;
class GravityComponent;
class ModelComponent;

class MovementComponent : public Component
{
public:
    // 更新関数
    void Update(float elapsed_time)override;
    // 名前取得
    const char* GetName()const override { return "MovementComponent"; };
    // 優先度
    const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::VERY_HIGH; }

    // XZ軸方向に移動しているか
    bool IsMoveXZAxis();

    // 各種設定取得・関数
    void SetMoveVec(DirectX::XMFLOAT3 move_vec) { this->move_vec = move_vec; }
    void SetMoveVecX(float move_vecX) { this->move_vec.x = move_vecX; }
    void SetMoveVecY(float move_vecY) { this->move_vec.y = move_vecY; }
    void SetMoveVecZ(float move_vecZ) { this->move_vec.z = move_vecZ; }
    void SetMoveSpeed(float speed) { this->speed = speed; }
    void SetIsStageRaycas(bool is_stage_raycas) { this->is_stage_raycas = is_stage_raycas; }
    const DirectX::XMFLOAT3& GetMoveVec() { return this->move_vec; }
    const DirectX::XMFLOAT3& GetVelocity() { return this->velocity; }
    const float& GetMoveVecX() { return this->move_vec.x; }
    const float& GetMoveVecY() { return this->move_vec.y; }
    const float& GetMoveVecZ() { return this->move_vec.z; }
    const float& GetSpeed() { return this->speed; }

private:
    DirectX::XMFLOAT3 velocity{};   // 速度
    DirectX::XMFLOAT3 move_vec{};   // 移動方向のベクトル
    float speed = 1.0f;             // 移動速度
    float step_offset = 0.2f;       // レイの開始位置を足元より少し上に設定するためのオフセット
    bool is_stage_raycas = false;   // ステージとのレイキャストの有無

private:
    std::weak_ptr<Transform3DComponent> transform_Wptr;
    std::weak_ptr<GravityComponent> gravity_Wptr;

    std::weak_ptr<ModelComponent> stage_model_Wptr;     // ステージのモデル
#ifdef _DEBUG
public:
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI();

#endif _DEBUG
};

