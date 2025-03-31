#pragma once
#include <memory>
#include "System/MyMath/MYVECTOR3.h"
#include "Component.h"
#ifdef _DEBUG
#include "Debug/DebugPrimitiveRenderer.h"
#endif // _DEBUG


class Transform3DComponent;
class GravityComponent;
class ModelComponent;

class MovementComponent : public Component
{
public:
    struct MovementParam
    {
        DirectX::XMFLOAT3 velocity{};               // 速度
        DirectX::XMFLOAT3 acceleration{};           // 加速度
        float step_offset = 1.0f;                   // レイの開始位置を足元より少し上に設定するためのオフセット
        float max_accelerationXZ = 5.0f;            // XZ軸の最大加速度
        float turn_speed = 15.0f;                   // 回転速度
        bool is_stage_raycast = false;              // ステージとのレイキャストの有無

        float push_rate = 1.0f;
    };
public:
    MovementComponent(MovementParam param);

    // リスタート処理
    void ReStart() override { this->param = default_param; };      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time)override;
    // 名前取得
    const char* GetName()const override { return "MovementComponent"; };
    // 優先度
    const PRIORITY GetPriority()const noexcept override { return PRIORITY::HIGH; }

    // 移動方向に向く
    void FaceMovementDirection(float elapsed_timed);

    // XZ軸方向に移動しているか
    bool IsMoveXZAxis()  const;

    // 各種設定取得・関数
    void SetIsStageRaycast(bool is_stage_raycast) { this->param.is_stage_raycast = is_stage_raycast; }
    void SetAdditionalVelocity(DirectX::XMFLOAT3 move_vec) { this->param.acceleration = move_vec; }
    void AddAcceleration(MYVECTOR3 Add_acc);
    void AddAccelerationXZ(float x, float z);
    void AddAccelerationX(float x);
    void AddAccelerationY(float y);
    void AddAccelerationZ(float z);
    DirectX::XMFLOAT3 GetAcceleration() const { return this->param.acceleration; }
    DirectX::XMFLOAT3 GetVelocity() const { return this->param.velocity; }
    float GetMaxAccelerationXZ() const { return this->param.max_accelerationXZ; }

    // ステージとのレイキャスト
    void RaycastVsStage(std::shared_ptr<Object> owner,std::shared_ptr<Transform3DComponent>& transform);

    // セルとの衝突判定を行う
    void ResolveGridCellCollision(std::shared_ptr<Object> owner, std::shared_ptr<Transform3DComponent>& transform,float elapsed_time);
private:
    MovementParam param;
    MovementParam default_param;

    int old_cell_index = -1;            // 前フレームのセルインデックス
    int current_cell_index = -1;        // セルインデックス
private:
    std::weak_ptr<Transform3DComponent> transform_Wptr;
    std::weak_ptr<GravityComponent> gravity_Wptr;

    std::weak_ptr<ModelComponent> stage_floor_model_Wptr;     // ステージのモデル
    std::weak_ptr<ModelComponent> stage_wall_model_Wptr;     // ステージのモデル
#ifdef _DEBUG
public:
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI() override;
    /**
     * デバックの情報を3D画面上に出力する関数
     */
    void DrawDebugPrimitive()override;
    /**
     * デバッグプリミティブ表示用ImGui
     */
     void DrawDebugPrimitiveGUI() override;
    // デバッグプリミティブを表示する
    bool IsDebugPrimitive()override { return true; }
private:
    SphereParam rayY_start_pos;   // 下方向のレイキャストの始点
    SphereParam rayY_end_pos;     // 下方向のレイキャストの終点
    SphereParam rayXZ_start_pos;  // 壁方向のレイキャストの始点
    SphereParam rayXZ_end_pos;    // 壁方向のレイキャストの終点
#endif _DEBUG
};
