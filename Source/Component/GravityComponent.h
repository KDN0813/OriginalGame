#pragma once
#include "Component.h"

class MovementComponent;

class GravityComponent : public Component
{
public:
    GravityComponent() {};

    // �X�V�֐�
    void Update(float elapsed_time);

    // ���O�擾
    const char* GetName()const { return "GravityComponent"; };
    // �D��x
    const COMPONENT_PRIORITY GetPriority()const noexcept { return COMPONENT_PRIORITY::CRITICAL; };


    // �e��擾�E�ݒ�֐�
    const float& GetGravity() { return this->gravity; }
    const bool& GetIsGrounded() { return this->is_grounded; }
    void SetIsGrounded(bool is_grounded) { this->is_grounded = is_grounded; }
private:
    float gravity = -0.5f;   // �d�͉����x
    bool is_grounded = false;// �n�ʂɂ��邩
private:
    std::weak_ptr<MovementComponent> movement_Wptr;

#ifdef _DEBUG
public:
    void DrawDebugGUI();

#endif // _DEBUG
};