#pragma once
#include "Component.h"
#include <memory>

class Transform3DComponent;
class GravityComponent;
class MovementComponent;
class ModelComponent;

// �X�e�[�W�Ƃ̃��C�L���X�g���s��
class StageRaycastComponent : public Component
{
public:
    StageRaycastComponent() {};

   void Update(float elapsed_time) override;

    // ���O�擾
   const char* GetName()const override { return "StageRaycastComponent"; };
   // �D��x(�����l�͍Œ�l)
   const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::HIGH; };

   // �e��ݒ�E�擾�֐�
   const float& GetStep0ffset() { return step_offset; }
   void SetStep0ffset(float step_offset) { this->step_offset = step_offset; }
private:
    float step_offset = 0.2f;   // ���C�̊J�n�ʒu�𑫌���菭����ɐݒ肷�邽�߂̃I�t�Z�b�g

private:
    std::weak_ptr<Transform3DComponent> transform_Wptr;
    std::weak_ptr<GravityComponent> gravity_Wptr;
    std::weak_ptr<MovementComponent> movement_Wptr;

    // �X�e�[�W�̃��f��
    std::weak_ptr<ModelComponent> stage_model_Wptr;
#ifdef _DEBUG
public:
    void DrawDebugGUI();
    void DrawDebugPrimitive();
#endif // _DEBUG
};

