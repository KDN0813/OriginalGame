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
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::LOWEST; };

    // ���I�u�W�F�N�g�ɐڐG�������̏���
    void OnCollision(const std::shared_ptr<Object>& hit_object)override;

    // �q�b�g�����������u�Ԃ��ǂ���
    bool IsHitTriggered() const { return true; }

    // �e��擾�E�ݒ�֐�
    COLLISION_TYPE GetCollisionType() const { return this->param.collision_type; }
    float GetRadius() const { return this->param.radius; }
    CircleParam GetCircleParam();
    CircleHitResult GetCircleHitResult()const { return this->hit_result; }
    void SetCollisionType(COLLISION_TYPE type) { this->param.collision_type = type; }
    void SetRadius(float radius) { this->param.radius = radius; }
    void SetHitResult(CircleHitResult result) { this->hit_result = result; }

private:
    // �ڐG�����u�ԌĂ΂��֐�
    void OnCollisionEnter(const std::shared_ptr<Object>& hit_object);
    // �ڐG���Ă���ԌĂ΂��֐�
    void OnCollisionStay(const std::shared_ptr<Object>& hit_object);

private:
    CollisionParam param;
    CollisionParam default_param;
    CircleHitResult hit_result{};

    std::vector<std::weak_ptr<Component>> collision_enter_component_Wptr_pool;  // �ڐG�����u�ԏ�������R���|�[�l���g
    std::vector<std::weak_ptr<Component>> collision_stay_component_Wptr_pool;   // �ڐG���Ă���ԏ�������R���|�[�l���g
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

