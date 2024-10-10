#pragma once
#include "Component.h"
#include "System/MyMath/MYVECTOR3.h"

class MovementComponent;
class Transform3DComponent;

class PlayerComponent : public Component
{
public:
    // çXêVä÷êî
    void Update(float elapsed_time) override;
    // ñºëOéÊìæ
    const char* GetName()const override { return "PlayerComponent"; };
    const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::CRITICAL; };

private:
    bool InputMove(float elapsed_time);
    void Move(float vx, float vz, float speed);
    void Turn(float elapsed_time, float vx, float vz, float speed);
    MYVECTOR3 GetMoveVec() const;

private:
    float move_speed = 10.0f;
    float turn_speed = 15.0f;

private:
    std::weak_ptr<MovementComponent> movement_Wptr;
    std::weak_ptr<Transform3DComponent> transform_Wptr;

#ifdef _DEBUG
public:
    void DrawDebugGUI() override;
#endif // _DEBUG
};

