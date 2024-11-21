#pragma once
#include "Component.h"
#include <DirectXMath.h>
#ifdef _DEBUG
#include "Debug/DebugPrimitiveRenderer.h"
#endif // _DEBUG

class Transform3DComponent;

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
    void Update(float elapsed_time) override;

    // ���O�擾
    const char* GetName()const  override { return "DebugParticle"; };

    // �D��x
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::DEFAULT; };
private:
    // �͈͓��̃����_���Ȉʒu�ɃG�t�F�N�g���Đ�������
    void PlayEffect(int type);

private:
    float effect_area_radius = 2.0f;    // �G�t�F�N�g���Đ�����G���A(�~��)�̔��a
    float effect_area_height = 2.0f;    // �G�t�F�N�g���Đ�����G���A(�~��)�̍���
    DirectX::XMFLOAT3 area_pos = {};    // �G�t�F�N�g���Đ�����G���A�̒��S
    bool effect_looping = false;    // �G�t�F�N�g�����[�v�Đ�������t���O

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
    bool IsDebugPrimitive() override { return true; }   // DebugPrimitive�����݂��邩
private:
    CylinderParam debug_cylinder_effect_area;
    size_t group_count = 100;       // �Đ�����O���[�v�G�t�F�N�g�̐�
    int draw_effect_type = 0;   // �f�o�b�O�ŕ`�悷��G�t�F�N�g�̃^�C�v
    bool is_random_spawn_position = false;	// �p�[�e�B�N���̐����ʒu�������_���ɂ��邩�̃t���O
#endif // DEBUG
};

