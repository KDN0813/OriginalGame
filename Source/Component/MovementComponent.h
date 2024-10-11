#pragma once
#include <memory>
#include "System/MyMath/MYVECTOR3.h"
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
    bool IsMoveXZAxis()  const;

    // 各種設定取得・関数
    void SetIsStageRaycas(bool is_stage_raycas) { this->is_stage_raycas = is_stage_raycas; }
    void SetAdditionalVelocity(DirectX::XMFLOAT3 move_vec) { this->acceleration = move_vec; }
    void AddAcceleration(MYVECTOR3 Add_acc);
    void AddAccelerationXZ(float x, float z);
    void AddAccelerationX(float x);
    void AddAccelerationY(float y);
    void AddAccelerationZ(float z);
    DirectX::XMFLOAT3 GetAcceleration() { return this->acceleration; }
    DirectX::XMFLOAT3 GetVelocity() { return this->velocity; }

    // ステージとのレイキャスト
    void RaycasVsStage(std::shared_ptr<Object> owner,std::shared_ptr<Transform3DComponent>& transform);

private:
    DirectX::XMFLOAT3 velocity{};               // 速度
    DirectX::XMFLOAT3 acceleration{};    // 加速度
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

