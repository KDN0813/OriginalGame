#pragma once
#include "Component.h"

class Transform3DComponent;

class GravityComponent : public Component
{
public:
    GravityComponent() {};

    // 更新関数
    void Update(float elapsed_time);

    // 名前取得
    const char* GetName()const { return "GravityComponent"; };
    // 優先度
    const COMPONENT_PRIORITY GetPriority()const noexcept { return COMPONENT_PRIORITY::VERY_HIGH; };


    // 各種取得・設定関数
    const float& GetGravity() { return this->gravity; }
    const bool& GetIsGrounded() { return this->is_grounded; }
    void SetIsGrounded(bool is_grounded) { this->is_grounded = is_grounded; }
private:
    float gravity = -0.5f;   // 重力加速度
    bool is_grounded = true;
private:
    std::weak_ptr<Transform3DComponent> transform_Wptr;

#ifdef _DEBUG
public:
    void DrawDebugGUI();

#endif // _DEBUG
};