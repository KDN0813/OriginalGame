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
        DirectX::XMFLOAT3 velocity{};               // ���x
        DirectX::XMFLOAT3 acceleration{};           // �����x
        float step_offset = 1.0f;                   // ���C�̊J�n�ʒu�𑫌���菭����ɐݒ肷�邽�߂̃I�t�Z�b�g
        float max_accelerationXZ = 5.0f;            // XZ���̍ő�����x
        float turn_speed = 15.0f;                   // ��]���x
        bool is_stage_raycas = false;               // �X�e�[�W�Ƃ̃��C�L���X�g�̗L��
    };
public:
    MovementComponent(MovementParam param);

    // ���X�^�[�g����
    void ReStart() override { this->param = default_param; };      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time)override;
    // ���O�擾
    const char* GetName()const override { return "MovementComponent"; };
    // �D��x
    const PRIORITY GetPriority()const noexcept override { return PRIORITY::HIGH; }

    // �ړ������Ɍ���
    void FaceMovementDirection(float elapsed_timed);

    // XZ�������Ɉړ����Ă��邩
    bool IsMoveXZAxis()  const;

    // �e��ݒ�擾�E�֐�
    void SetIsStageRaycas(bool is_stage_raycas) { this->param.is_stage_raycas = is_stage_raycas; }
    void SetAdditionalVelocity(DirectX::XMFLOAT3 move_vec) { this->param.acceleration = move_vec; }
    void AddAcceleration(MYVECTOR3 Add_acc);
    void AddAccelerationXZ(float x, float z);
    void AddAccelerationX(float x);
    void AddAccelerationY(float y);
    void AddAccelerationZ(float z);
    DirectX::XMFLOAT3 GetAcceleration() const { return this->param.acceleration; }
    DirectX::XMFLOAT3 GetVelocity() const { return this->param.velocity; }
    float GetMaxAccelerationXZ() const { return this->param.max_accelerationXZ; }

    // �X�e�[�W�Ƃ̃��C�L���X�g
    void RaycasVsStage(std::shared_ptr<Object> owner,std::shared_ptr<Transform3DComponent>& transform);

private:
    MovementParam param;
    MovementParam default_param;

private:
    std::weak_ptr<Transform3DComponent> transform_Wptr;
    std::weak_ptr<GravityComponent> gravity_Wptr;

    std::weak_ptr<ModelComponent> stage_foor_model_Wptr;     // �X�e�[�W�̃��f��
    std::weak_ptr<ModelComponent> stage_wall_model_Wptr;     // �X�e�[�W�̃��f��
#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI() override;
    /**
     * �f�o�b�N�̏���3D��ʏ�ɏo�͂���֐�
     */
    void DrawDebugPrimitive()override;
    /**
     * �f�o�b�O�v���~�e�B�u�\���pImGui
     */
     void DrawDebugPrimitiveGUI() override;
    // �f�o�b�O�v���~�e�B�u��\������
    bool IsDebugPrimitive()override { return true; }
private:
    SphereParam rayY_start_pos;   // �������̃��C�L���X�g�̎n�_
    SphereParam rayY_end_pos;     // �������̃��C�L���X�g�̏I�_
    SphereParam rayXZ_start_pos;  // �Ǖ����̃��C�L���X�g�̎n�_
    SphereParam rayXZ_end_pos;    // �Ǖ����̃��C�L���X�g�̏I�_
#endif _DEBUG
};

