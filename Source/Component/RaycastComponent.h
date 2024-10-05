#pragma once
#include "Component.h"
#include <memory>

class Transform3DComponent;
class GravityComponent;
class MovementComponent;
class ModelComponent;

// ステージとのレイキャストを行う
class StageRaycastComponent : public Component
{
public:
    StageRaycastComponent() {};

   void Update(float elapsed_time) override;

    // 名前取得
   const char* GetName()const override { return "StageRaycastComponent"; };
   // 優先度(初期値は最低値)
   const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::HIGH; };

   // 各種設定・取得関数
   const float& GetStep0ffset() { return step_offset; }
   void SetStep0ffset(float step_offset) { this->step_offset = step_offset; }
private:
    float step_offset = 0.2f;   // レイの開始位置を足元より少し上に設定するためのオフセット

private:
    std::weak_ptr<Transform3DComponent> transform_Wptr;
    std::weak_ptr<GravityComponent> gravity_Wptr;
    std::weak_ptr<MovementComponent> movement_Wptr;

    // ステージのモデル
    std::weak_ptr<ModelComponent> stage_model_Wptr;
#ifdef _DEBUG
public:
    void DrawDebugGUI();
    void DrawDebugPrimitive();
#endif // _DEBUG
};

