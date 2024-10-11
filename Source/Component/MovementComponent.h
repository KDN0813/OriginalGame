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
    // �X�V�֐�
    void Update(float elapsed_time)override;
    // ���O�擾
    const char* GetName()const override { return "MovementComponent"; };
    // �D��x
    const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::VERY_HIGH; }

    // XZ�������Ɉړ����Ă��邩
    bool IsMoveXZAxis()  const;

    // �e��ݒ�擾�E�֐�
    void SetIsStageRaycas(bool is_stage_raycas) { this->is_stage_raycas = is_stage_raycas; }
    void SetAdditionalVelocity(DirectX::XMFLOAT3 move_vec) { this->acceleration = move_vec; }
    void AddAcceleration(MYVECTOR3 Add_acc);
    void AddAccelerationXZ(float x, float z);
    void AddAccelerationX(float x);
    void AddAccelerationY(float y);
    void AddAccelerationZ(float z);
    DirectX::XMFLOAT3 GetAcceleration() { return this->acceleration; }
    DirectX::XMFLOAT3 GetVelocity() { return this->velocity; }

    // �X�e�[�W�Ƃ̃��C�L���X�g
    void RaycasVsStage(std::shared_ptr<Object> owner,std::shared_ptr<Transform3DComponent>& transform);

private:
    DirectX::XMFLOAT3 velocity{};               // ���x
    DirectX::XMFLOAT3 acceleration{};    // �����x
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

