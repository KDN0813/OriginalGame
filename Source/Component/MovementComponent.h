#pragma once
#include <memory>
#include <DirectXMath.h>
#include "Component.h"

class Transform3DComponent;

class MovementComponent : public Component
{
public:
    // �X�V�֐�
    void Update(float elapsed_time)override;
    // ���O�擾
    const char* GetName()const override { return "MovementComponent"; };
    // �D��x
    const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::VERY_HIGH; }

    // XZ�������Ɉړ����Ă��邩
    bool IsMoveXZAxis();

    // �e��ݒ�擾�E�֐�
    void SetVelocity(DirectX::XMFLOAT3 move_vec) { this->move_vec = move_vec; }
    void SetVelocityX(float move_vecX) { this->move_vec.x = move_vecX; }
    void SetVelocityY(float move_vecY) { this->move_vec.y = move_vecY; }
    void SetVelocityZ(float move_vecZ) { this->move_vec.z = move_vecZ; }
    void SetMoveSpeed(float speed) { this->speed = speed; }
    const float& GetVelocityX() { return this->move_vec.x; }
    const float& GetVelocityY() { return this->move_vec.y; }
    const float& GetVelocityZ() { return this->move_vec.z; }

private:
    DirectX::XMFLOAT3 move_vec{};   // �ړ������̃x�N�g��
    float speed = 1.0f;             // �ړ����x

private:
    std::weak_ptr<Transform3DComponent> transform3D_Wptr;

#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI();

#endif _DEBUG
};

