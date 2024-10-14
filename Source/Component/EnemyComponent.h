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

    void Move(float vx, float vz, float speed);
    void Turn(float elapsed_time, float vx, float vz, float speed);
    void MoveToTarget(float elapsed_time, std::shared_ptr<Transform3DComponent>& transform,float speed_rate);
    void SetRandomTargetPosition();
private:
    DirectX::XMFLOAT3 target_position{};
    DirectX::XMFLOAT3 territory_origin{};
    float territory_range = 30.0f;
    float radius = 5.0f;
    float move_speed = 3.0f;
    float speed_rate = 0.5f;
    float turn_speed = DirectX::XMConvertToRadians(360);

private:
    std::weak_ptr<MovementComponent> movement_Wptr;
    std::weak_ptr<Transform3DComponent> transform_Wptr;
#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI() override;
#endif // DEBUG
};

