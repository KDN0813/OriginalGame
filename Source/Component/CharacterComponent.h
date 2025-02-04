#pragma once
#include "Component.h"

#ifdef _DEBUG
#include "Debug\DebugPrimitiveRenderer.h"
class Transform3DComponent;
#endif // _DEBUG

class CharacterComponent : public Component
{
public:
    struct CharacterParam
    {
        int hp = 1;     // �̗�
        int old_hp = 1; // �O�t���[���̗̑�
        int max_hp = 1; // �ő�̗�
        float invincible_timer = 0.0f;      // ���G����
        bool invincible_flag = false;       // ���G�t���O(�i���I�ɖ��G��Ԃɂ���ۂɎg�p����)
        float radius = 0.5f;                // �L�����N�^�[�̔��a
    };
public:
    CharacterComponent(CharacterParam param);
    ~CharacterComponent() {};

    // �J�n�֐�
    void Start()  override {};
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override;      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override;

    // ���O�擾
    const char* GetName()const  override { return "CharacterComponent"; };

    // �_���[�W��K�p����
    void ApplyDamage(int damage, float invincible_timer);
    // �������Ă��邩
    bool IsAlive();
    // �_���[�W���󂯂���
    bool IsDamage();

    // ���G��Ԃł��邩
    bool IsInvincible();

    // �e��擾�E�ݒ�֐�
    int GetHP()const { return this->param.hp; }
    int GetMaxHP()const { return this->param.max_hp; }
    float GetRadius()const { return this->param.radius; }
    // �̗͂̊������擾
    float GetHealthPercentage() { return static_cast<float>(this->param.hp) / static_cast<float>(this->param.max_hp); };
    bool GetInvincibleFlag()const { return this->param.invincible_flag; }
    void SetInvincibleFlag(const bool flag) { this->param.invincible_flag = flag; }

    // �D��x
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::LOWEST; };
private:
    CharacterParam param;
    CharacterParam default_param;

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
    bool IsDebugPrimitive() override { return true; }   // DebugPrimitive�����݂��邩

private:
    int test_damage = 1;    // �e�X�g�ŗ^����_���[�W��
    CylinderParam character_radius{};
    std::weak_ptr<Transform3DComponent> transform_Wptr;
#endif // DEBUG
};

