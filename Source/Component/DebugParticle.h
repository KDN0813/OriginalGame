#pragma once
#include "Component.h"
#include <DirectXMath.h>
#ifdef _DEBUG
#include "Debug/DebugPrimitiveRenderer.h"
#endif // _DEBUG


// ParticleSystem�������邽�߂̃f�o�b�O�p�R���|�[�l���g
class DebugParticle : public Component
{
public:
    DebugParticle();
    ~ DebugParticle() {};

    // �J�n�֐�
    void Start()  override {};
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override {};      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override {};

    // ���O�擾
    const char* GetName()const  override {};

    // �D��x
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::DEFAULT; };

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
    CylinderParam debug_cylinder_effect_area;
    float effect_area_radius = 2.0f;    // �G�t�F�N�g���Đ�����G���A(�~��)�̔��a
    float effect_area_height = 2.0f;    // �G�t�F�N�g���Đ�����G���A(�~��)�̍���
#endif // DEBUG
};

