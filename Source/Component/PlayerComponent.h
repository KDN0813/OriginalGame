#pragma once
#include "Component.h"
#include <DirectXMath.h>

class MovementComponent;

class PlayerComponent : public Component
{
public:
    // XVŠÖ”
    void Update(float elapsed_time) override;
    // –¼‘Oæ“¾
    const char* GetName()const override { return "PlayerComponent"; };
    const unsigned int GetPriority()const noexcept override { return 3; };

private:
    bool InputMove();
    void Move(float vx, float vz, float speed);
    DirectX::XMFLOAT3 GetMoveVec() const;

private:
    float move_speed = 10.0f;

private:
    std::weak_ptr<MovementComponent> movement_Wptr;

#ifdef _DEBUG
public:
    void DrawDebugGUI() override;
#endif // _DEBUG
};

