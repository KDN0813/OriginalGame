#pragma once
#include "Component.h"
#include "Collision/CollisionParam.h"

class Transform3DComponent;

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
    COLLISION_TYPE GetCollisionType() const { return this->collision_type; }
    TARGET_TYPE GetTargetType() const { return this->target_type; }
    float GetRadius() const { return this->radius; }
    CircleParam GetCircleParam();
    bool GetHitFlag() const { return this->hit_flag; }
    void SetCollisionType(COLLISION_TYPE type) { this->collision_type = type; }
    void SetTargetType(TARGET_TYPE type) { this->target_type = type; }
    void SetRadius(float radius) { this->radius = radius; }
    void SetHitFlag(bool hit) { this->hit_flag = hit; }
    void SetHitResult(CircleHitResult result) { this->hit_result = result; }

private:
    COLLISION_TYPE collision_type = COLLISION_TYPE::NONE;
    TARGET_TYPE target_type = TARGET_TYPE::NONE;
    float radius = 1.0f;    // �~�̔��a
    bool hit_flag = false;
    CircleHitResult hit_result;
private:
    std::weak_ptr<Transform3DComponent> transform_Wptr;
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

