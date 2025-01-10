#pragma once
#include "Component.h"

class CharacterComponent : public Component
{
public:
    struct CharacterParam
    {
        int hp = 1;     // �̗�
        int max_hp = 1; // �ő�̗�
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
    void Update(float elapsed_time) override {};

    // ���O�擾
    const char* GetName()const  override { return "CharacterComponent"; };

    // �_���[�W��K�p����
    void ApplyDamage(int damege);
    // �������Ă��邩
    bool IsAlive();

    // �e��擾�֐�
    int GetHP()const { return this->param.hp; }
    int GetMaxHP()const { return this->param.max_hp; }
    // �̗͂̊������擾
    float GetHealthPercentage() { return static_cast<float>(this->param.hp) / static_cast<float>(this->param.max_hp); };

    // �D��x
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };
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
    void DrawDebugPrimitive()  override {};
    /**
     * �f�o�b�O�v���~�e�B�u�\���pImGui
     */
    void DrawDebugPrimitiveGUI()  override {};
    bool IsDebugPrimitive() override { return false; }   // DebugPrimitive�����݂��邩

private:
    int test_damage = 1;    // �e�X�g�ŗ^����_���[�W��
#endif // DEBUG
};

