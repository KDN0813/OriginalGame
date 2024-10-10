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
    bool IsMoveXZAxis();

    // 各種設定取得・関数
    void SetIsStageRaycas(bool is_stage_raycas) { this->is_stage_raycas = is_stage_raycas; }
    void SetAdditionalVelocity(MYVECTOR3 move_vec) { this->acceleration = move_vec; }
    void AddAcceleration(MYVECTOR3 add_acc) { this->acceleration += add_acc; }
    void AddAcceleration(float x, float y, float z) { this->acceleration += MYVECTOR3(x, y, z); }
    void AddAccelerationXZ(float x, float z) { this->acceleration += MYVECTOR3(x, 0.0f, z); }
    void AddAccelerationX(float x) { this->acceleration += MYVECTOR3(x, 0.0f, 0.0f); }
    void AddAccelerationY(float y) { this->acceleration += MYVECTOR3(0.0f, y, 0.0f); }
    void AddAccelerationZ(float z) { this->acceleration += MYVECTOR3(0.0f, 0.0f, z); }
    MYVECTOR3 GetAcceleration() { return this->acceleration; }
    MYVECTOR3 GetVelocity() { return this->velocity; }

    // ステージとのレイキャスト
    void RaycasVsStage(std::shared_ptr<Object> owner,std::shared_ptr<Transform3DComponent>& transform);

private:
    MYVECTOR3 velocity{};               // 速度
    MYVECTOR3 acceleration{};    // 加速度
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

