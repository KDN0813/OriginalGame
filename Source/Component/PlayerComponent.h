#pragma once
#include "Component.h"
#include <DirectXMath.h>

class MovementComponent;
class Transform3DComponent;

class PlayerComponent : public Component
{
public:
    // �X�V�֐�
    void Update(float elapsed_time) override;
    // ���O�擾
    const char* GetName()const override { return "PlayerComponent"; };
    const unsigned int GetPriority()const noexcept override { return 3; };

private:
    bool InputMove(float elapsed_time);
    void Move(float vx, float vz, float speed);
    void Turn(float elapsed_time, float vx, float vz, float speed);
    DirectX::XMFLOAT3 GetMoveVec() const;

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
