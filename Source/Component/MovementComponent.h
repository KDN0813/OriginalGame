#pragma once
#include <memory>
#include <DirectXMath.h>
#include "Component.h"

class Transform3DComponent;
class GravityComponent;
class ModelComponent;

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
    void SetAdditionalVelocity(DirectX::XMFLOAT3 move_vec) { this->additional_velocity = move_vec; }
    void SetAdditionalVelocityX(float move_vecX) { this->additional_velocity.x = move_vecX; }
    void SetAdditionalVelocityY(float move_vecY) { this->additional_velocity.y = move_vecY; }
    void SetAdditionalVelocityZ(float move_vecZ) { this->additional_velocity.z = move_vecZ; }
    void AddAdditionalVelocityX(float move_vecX) { this->additional_velocity.x += move_vecX; }
    void AddAdditionalVelocityY(float move_vecY) { this->additional_velocity.y += move_vecY; }
    void AddAdditionalVelocityZ(float move_vecZ) { this->additional_velocity.z += move_vecZ; }
    void SetIsStageRaycas(bool is_stage_raycas) { this->is_stage_raycas = is_stage_raycas; }
    const DirectX::XMFLOAT3& GetAdditionalVelocity() { return this->additional_velocity; }
    const DirectX::XMFLOAT3& GetVelocity() { return this->velocity; }
    const float& GetMoveVecX() { return this->additional_velocity.x; }
    const float& GetMoveVecY() { return this->additional_velocity.y; }
    const float& GetMoveVecZ() { return this->additional_velocity.z; }

private:
    DirectX::XMFLOAT3 velocity{};               // ���x
    DirectX::XMFLOAT3 additional_velocity{};    // �����x
    float step_offset = 0.2f;                   // ���C�̊J�n�ʒu�𑫌���菭����ɐݒ肷�邽�߂̃I�t�Z�b�g
    float max_accelerationXZ = 5.0f;            // XZ���̍ő�����x
    bool is_stage_raycas = false;               // �X�e�[�W�Ƃ̃��C�L���X�g�̗L��

private:
    std::weak_ptr<Transform3DComponent> transform_Wptr;
    std::weak_ptr<GravityComponent> gravity_Wptr;

    std::weak_ptr<ModelComponent> stage_model_Wptr;     // �X�e�[�W�̃��f��
#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI();

#endif _DEBUG
};

