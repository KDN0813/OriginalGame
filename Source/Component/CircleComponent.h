#pragma once
#include "Component.h"
#include "Collision/CollisionParam.h"

// �~�̓����蔻��p�R���|�[�l���g
class CircleComponent : public Component
{
public:
    CircleComponent() {};
    virtual ~CircleComponent() {};

    // �J�n�֐�
    void Start()  override;
    // �I���֐�
    void End()  override;
    // �X�V�֐�
    void Update(float elapsed_time) override;

    // ���O�擾
    const char* GetName()const  override { return "CircleComponent"; };

    // �D��x
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::CRITICAL; };

    // �e��擾�E�ݒ�֐�
    CollisionType GetCollisionType() { return this->collision_type; }
    TargetType GetTargetType() { return this->target_type; }
    float GetRadius() { return this->radius; }
    void SetCollisionType(COLLISION_TYPE type) { this->collision_type = type; }
    void SetTargetType(TARGET_TYPE type) { this->target_type = type; }
    void SetRadius(float radius) { this->radius = radius; }

private:
    CollisionType collision_type = COLLISION_TYPE::COLLISION_TYPE_DEFAULT;
    TargetType target_type = TARGET_TYPE::TARGET_DEFAULT;
    float radius = 1.0f;    // �~�̔��a

#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI()  override {};
    /**
     * �f�o�b�N�̏���3D��ʏ�ɏo�͂���֐�
     */
    void DrawDebugPrimitive()  override {};
    /**
     * �f�o�b�O�v���~�e�B�u�\���pImGui
     */
    void DrawDebugPrimitiveGUI()  override {};
    bool IsDebugPrimitive() { return false; }   // DebugPrimitive�����݂��邩
#endif // DEBUG
};

