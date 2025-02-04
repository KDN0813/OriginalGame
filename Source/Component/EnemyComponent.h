#pragma once
#include "Component.h"
#include "DirectXMath.h"
#include "Object\Constant\EnemyConstant.h"

class Transform3DComponent;
class MovementComponent;
class CircleCollisionComponent;
class CharacterComponent;
class InstancedModelWithAnimationComponent;

class EnemyComponent : public Component
{
public:
    enum class STATE
    {
        IDLE = 0,   // �ҋ@
        WANDERING,  // ���Q
        CHASE,      // �ڋ�
        ATTACK,     // �U��
        DAMAGE,     // �_���[�W
        DETH,       // ���S
    };

    struct EnemyParam
    {
        DirectX::XMFLOAT3 target_position{};
        float territory_range = EnemyConstant::DEFAULT_TERRITORY_RENGR;
        float radius = 0.1f;
        float move_speed = 5.0f;
        float speed_rate = 0.5f;

        float attack_range = 3.0f;    // �U���͈�

        float idle_timer = 0.0f;    // �ҋ@����
        float max_idle_time = 5.0f;
        float min_idle_time = 0.5f;
        float remove_timer = 2.0f;    // �폜�^�C�}�[
        float damage_effect_offset_up = 0.5f;   // �_���[�W�G�t�F�N�g���󂯂鎞�̏������offset�l
        int damage_amount = 1;
        bool move_validity_flag = true;   // �ړ����L���ł��邩�̃t���O
        bool pending_removal_flag = false;     // �폜�҂������Ă��邩(���S���o��҂��Ă��邩)
        STATE state = STATE::IDLE;  // ���
        DirectX::XMFLOAT3 spawn_point{};    // �X�|�[���ʒu
    };
public:
    EnemyComponent(EnemyParam param) :param(param),default_param(param){};
    ~EnemyComponent() {};

    // �J�n�֐�
    void Start() override;
    // �I���֐�
    void End() override;
    // ���X�^�[�g����
    void ReStart() override { this->param = this->default_param; };      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override;

    // ���O�擾
    const char* GetName()const  override { return "EnemyComponent"; };

    // �D��x
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::VERY_HIGH; };

    // ���I�u�W�F�N�g�ɐڐG�������̏���
    void OnCollision(const std::shared_ptr<Object>& hit_object) override;

    // ���̖ړI�n�ݒ�
    void SetRandomTargetPosition();
    // ���̖ړI�n���v���C���[�̈ʒu�ɐݒ�
    void SetTargetPositionByPlayer();
    // �ҋ@���Ԑݒ�
    void SetRandomIdleTime();
    // �ړI�n�ɓ��B���Ă��邩
    bool IsAtTarget();
    bool IsAtTarget(float distSq);
    // �ړ��͈͓��Ƀv���C���[�����݂��邩
    bool IsPlayerInMovementArea();
    // �U���͈͓��Ƀv���C���[�����݂��邩
    bool IsPlayerInAttacklArea();

    // ���ݑҋ@�s�����ł��邩
    bool IsIdle() { return (this->param.idle_timer > 0.0f); }

    // �e��E�ݒ�擾�֐�
    bool GetMoveValidityFlag() const { return this->param.move_validity_flag; }
    float GetIdleTime() { return this->param.idle_timer; }
    void SetIdleTime(float time) { this->param.idle_timer = time; }
    void SetMoveValidityFlag(bool flag) { this->param.move_validity_flag = flag; }
    void SetPendingRemovalFlag(bool flag) { this->param.pending_removal_flag = flag; }
private:
    void Move(float vx, float vz, float speed);
    void MoveToTarget(float elapsed_time, std::shared_ptr<Transform3DComponent>& transform, float speed_rate);

    // �_���[�W�X�e�[�g�ɑJ��
    void SetDamageState();
private:
    EnemyParam param;
    EnemyParam default_param;
private:
    std::weak_ptr<MovementComponent> movement_Wptr;
    std::weak_ptr<Transform3DComponent> transform_Wptr;
    std::weak_ptr<CircleCollisionComponent> circle_collision_Wptr;
    std::weak_ptr<Transform3DComponent> child_transform_Wptr;
    std::weak_ptr<CharacterComponent> character_Wptr;
    std::weak_ptr<InstancedModelWithAnimationComponent> model_Wptr;
    std::weak_ptr<CircleCollisionComponent> child_collision_Wprt;   // �U���p�̃R���W����

    std::weak_ptr<Transform3DComponent> player_transform_Wptr;
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

