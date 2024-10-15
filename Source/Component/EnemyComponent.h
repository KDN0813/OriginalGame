#pragma once
#include "Component.h"
#include "DirectXMath.h"

class Transform3DComponent;
class MovementComponent;

class EnemyComponent : public Component
{
public:
    EnemyComponent() {};
    ~EnemyComponent() {};

    // �J�n�֐�
    void Start() override;
    // �I���֐�
    void End() override;
    // �X�V�֐�
    void Update(float elapsed_time) override;

    // ���O�擾
    const char* GetName()const  override { return "EnemyComponent"; };

    // �D��x
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::CRITICAL; };

    // ���̖ړI�n�ݒ�
    void SetRandomTargetPosition();
    // �ҋ@���Ԑݒ�
    void SetRandomIdleTime();
    // �ړI�n�ɓ��B���Ă��邩
    bool IsAtTarget();
    bool IsAtTarget(float distSq);
    // ���ݑҋ@�s�����ł��邩
    bool IsIdle() { return (this->idle_timer > 0.0f); }

    // �e��E�ݒ�擾�֐�
    float GetIdleTime() { return this->idle_timer; }
    void SetIdleTime(float time) { this->idle_timer = time; }
private:
    void Move(float vx, float vz, float speed);
    void MoveToTarget(float elapsed_time, std::shared_ptr<Transform3DComponent>& transform, float speed_rate);

private:
    DirectX::XMFLOAT3 target_position{};
    float territory_range = 45.0f;
    float radius = 1.0f;
    float move_speed = 3.0f;
    float speed_rate = 0.5f;

    float idle_timer = 0.0f;    // �ҋ@����
    float max_idle_time = 5.0f;
    float min_idle_time = 0.5f;
private:
    std::weak_ptr<MovementComponent> movement_Wptr;
    std::weak_ptr<Transform3DComponent> transform_Wptr;
#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI() override;
    /**
     * �f�o�b�N�̏���3D��ʏ�ɏo�͂���֐�
     */
    void DrawDebugPrimitive() override;
#endif // DEBUG
};

