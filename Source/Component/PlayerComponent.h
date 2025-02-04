#pragma once
#include <DirectXMath.h>
#include "Component.h"

class MovementComponent;
class Transform3DComponent;
class CircleCollisionComponent;
class CharacterComponent;
class CircleCollisionComponent;

class PlayerComponent : public Component
{
public:
    struct PlayerParam
    {
        float move_speed = 10.0f;
        int damage_amount = 1;
        bool input_move_validity_flag = true;   // ���͂ɂ��ړ����L���ł��邩�̃t���O
    
        float move_rate = 1.0f; // ���x�{��
        float spin_attack_move_rate = 2.0f;

        float spin_attack_use_point = 5.0f;     // ��]�U���ɕK�v�ȃ|�C���g(���b�R�X�g�K�p)

        float special_point = 0.0f;          // �X�y�V�����|�C���g
        float special_point_max = 100.0f;    // �X�y�V�����|�C���g�̍ő�l
    };
public:
    PlayerComponent(PlayerParam param);
    ~PlayerComponent();
    // ���X�^�[�g����
    void ReStart() override { this->param = this->default_param; };      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override;
    // ���O�擾
    const char* GetName()const override { return "PlayerComponent"; };
    const PRIORITY GetPriority()const noexcept override { return PRIORITY::VERY_HIGH; };

    bool GetInputMoveValidityFlag()const { return this->param.input_move_validity_flag; }
    void SetInputMoveValidityFlag(bool flag) { this->param.input_move_validity_flag = flag; }

    // ���I�u�W�F�N�g�ɐڐG�������̏���
    void OnCollision(const std::shared_ptr<Object>& hit_object)override;

    const float GetMoveRate()const { return this->param.move_rate; }
    const float GetSpinAttackMoveRate()const { return this->param.spin_attack_move_rate; }
    void SetMoveRate(const float move_rate) { this->param.move_rate = move_rate; }

    void AddSpecialPoint(float point) { this->param.special_point = (std::min)(this->param.special_point + point, this->param.special_point_max); }
    void SetSpecialPoint(float point) { this->param.special_point = (std::min)(point, this->param.special_point_max); }
    const float GetSpecialPoint()const { return this->param.special_point; }
    const float GetSpecialPointMax()const { return this->param.special_point_max; }
    // �X�y�V�����|�C���g���g�p����
    // �g�p�ł�����true��Ԃ�
    bool UseSpecialPoint(float use_point);
private:
    bool InputMove(float elapsed_time);
    void Move(float vx, float vz, float speed);
    DirectX::XMFLOAT3 GetMoveVec() const;

private:
    PlayerParam param;
    PlayerParam default_param;

public:
    // HACK �ϐ����E�ϐ��̈ʒu�ύX����
    // ���̎��Ԃ𒴂���Ƒ��X�e�[�g�ɑJ�ډ\�ɂȂ�
    float attack_end_point = 0.2f;
    float attack_combo2_end_point = 0.25f;
private:
    std::weak_ptr<MovementComponent> movement_Wptr;
    std::weak_ptr<Transform3DComponent> transform_Wptr;
    std::weak_ptr<Transform3DComponent> character_Wptr;
    std::weak_ptr<CircleCollisionComponent> circle_collision_Wptr;

#ifdef _DEBUG
public:
    void DrawDebugGUI() override;
    bool IsDebugPrimitive() override  { return false; }   // DebugPrimitive�����݂��邩

    static float debug_move_speed;    // Debug���̈ړ����x
#endif // _DEBUG

#ifdef RELEASE_DEBUG
public:
    void DebugCheat()override {};
#endif
};

