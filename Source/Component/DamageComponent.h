#pragma once
#include "Component.h"

class DamageComponent : public Component
{
public:
    struct DamageParam
    {
        int damage_amount = 1;          // �_���[�W��
        float invincible_time = 0.0f;   // ���G����
    };
public:
    DamageComponent(DamageParam param) : param(param) {};
    ~DamageComponent() {};

    // �J�n�֐�
    void Start()  override {};
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override {};      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override {};

    // ���O�擾
    const char* GetName()const  override { return "DamageComponent"; };

    // �D��x
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    int GetDamageAmount()const { return this->param.damage_amount; }
    float GetInvincibleTime()const { return this->param.invincible_time; }
private:
    DamageParam param;

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
#endif // DEBUG
};

