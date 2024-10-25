#pragma once
#include "Component.h"
#include "Collision/CollisionParam.h"
#ifdef _DEBUG
#include "Debug/DebugPrimitiveRenderer.h"
#endif // _DEBUG

class Transform3DComponent;

// �~�̓����蔻��p�R���|�[�l���g
class CircleCollisionComponent : public Component
{
public:
    struct CollisionParam
    {
        COLLISION_TYPE collision_type = COLLISION_TYPE::NONE;
        float radius = 1.0f;    // �~�̔��a
        bool default_active_flag = true;    // �A�N�e�B�u�t���O�̏����l
    };
public:
    CircleCollisionComponent(CollisionParam param);
    ~CircleCollisionComponent() {};

    // �J�n�֐�
    void Start()  override;
    // �I���֐�
    void End()  override;
    // ���X�^�[�g����
    void ReStart() override;      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override;

    // ���O�擾
    const char* GetName()const  override { return "CircleCollisionComponent"; };

    // �D��x
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::CRITICAL; };

    // �e��擾�E�ݒ�֐�
    COLLISION_TYPE GetCollisionType() const { return this->param.collision_type; }
    float GetRadius() const { return this->param.radius; }
    CircleParam GetCircleParam();
    bool GetHitFlag() const { return this->hit_flag; }
    void SetCollisionType(COLLISION_TYPE type) { this->param.collision_type = type; }
    void SetRadius(float radius) { this->param.radius = radius; }
    void SetHitFlag(bool hit) { this->hit_flag = hit; }
    void SetHitResult(CircleHitResult result) { this->hit_result = result; }

private:
    CollisionParam param;
    CollisionParam default_param;
    bool hit_flag = false;
    CircleHitResult hit_result{};
private:
    std::weak_ptr<Transform3DComponent> transform_Wptr;
#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI()  override;
    /**
     * �f�o�b�N�̏���3D��ʏ�ɏo�͂���֐�
     */
    void DrawDebugPrimitive()  override;
    /**
     * �f�o�b�O�v���~�e�B�u�\���pImGui
     */
    void DrawDebugPrimitiveGUI()  override;
    bool IsDebugPrimitive() { return true; }   // DebugPrimitive�����݂��邩
    CylinderParam circle_collsion_primitive;
#endif // DEBUG
private:
    float height = 1.3f;    // circle_collsion_primitive�̉~���̍���
};

