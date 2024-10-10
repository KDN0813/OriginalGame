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
    bool IsMoveXZAxis();

    // �e��ݒ�擾�E�֐�
    void SetIsStageRaycas(bool is_stage_raycas) { this->is_stage_raycas = is_stage_raycas; }
    void SetAdditionalVelocity(MYVECTOR3 move_vec) { this->acceleration = move_vec; }
    void AddAcceleration(MYVECTOR3 add_acc) { this->acceleration += add_acc; }
    void AddAcceleration(float x, float y, float z) { this->acceleration += MYVECTOR3(x, y, z); }
    void AddAccelerationXZ(float x, float z) { this->acceleration += MYVECTOR3(x, 0.0f, z); }
    void AddAccelerationX(float x) { this->acceleration += MYVECTOR3(x, 0.0f, 0.0f); }
    void AddAccelerationY(float y) { this->acceleration += MYVECTOR3(0.0f, y, 0.0f); }
    void AddAccelerationZ(float z) { this->acceleration += MYVECTOR3(0.0f, 0.0f, z); }
    MYVECTOR3 GetAcceleration() { return this->acceleration; }
    MYVECTOR3 GetVelocity() { return this->velocity; }

    // �X�e�[�W�Ƃ̃��C�L���X�g
    void RaycasVsStage(std::shared_ptr<Object> owner,std::shared_ptr<Transform3DComponent>& transform);

private:
    MYVECTOR3 velocity{};               // ���x
    MYVECTOR3 acceleration{};    // �����x
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

