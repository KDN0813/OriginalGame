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
    void SetAdditionalVelocity(DirectX::XMFLOAT3 move_vec) { this->additional_velocity = move_vec; }
    void SetAdditionalVelocityX(float move_vecX) { this->additional_velocity.x = move_vecX; }
    void SetAdditionalVelocityY(float move_vecY) { this->additional_velocity.y = move_vecY; }
    void SetAdditionalVelocityZ(float move_vecZ) { this->additional_velocity.z = move_vecZ; }
    void AddAdditionalVelocityX(float move_vecX) { this->additional_velocity.x += move_vecX; }
    void AddAdditionalVelocityY(float move_vecY) { this->additional_velocity.y += move_vecY; }
    void AddAdditionalVelocityZ(float move_vecZ) { this->additional_velocity.z += move_vecZ; }
    void SetIsStageRaycas(bool is_stage_raycas) { this->is_stage_raycas = is_stage_raycas; }
    const DirectX::XMFLOAT3& GetAdditionalVelocity() { return this->additional_velocity; }
    const DirectX::XMFLOAT3& GetVelocity() { return this->velocity; }
    const float& GetMoveVecX() { return this->additional_velocity.x; }
    const float& GetMoveVecY() { return this->additional_velocity.y; }
    const float& GetMoveVecZ() { return this->additional_velocity.z; }

private:
    DirectX::XMFLOAT3 velocity{};               // 速度
    DirectX::XMFLOAT3 additional_velocity{};    // 加速度
    float step_offset = 0.2f;                   // レイの開始位置を足元より少し上に設定するためのオフセット
    float max_accelerationXZ = 5.0f;            // XZ軸の最大加速度
    bool is_stage_raycas = false;               // ステージとのレイキャストの有無

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

