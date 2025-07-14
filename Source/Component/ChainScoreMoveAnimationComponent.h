#pragma once
#include "Component.h"
#include <DirectXMath.h>

class FadeControllerComponent;
class SpriteMoverComponent;

// �A���X�R�AUI�����v�X�R�A�ֈړ�������A�j���[�V�����𐧌䂷��R���|�[�l���g
class ChainScoreMoveAnimationComponent : public Component
{
public:
    struct Param
    {
        float fead_out_time = 1.0f;
        DirectX::XMFLOAT2 initial_pos = {}; // �����ʒu
        DirectX::XMFLOAT2 target_pos = {};  // �ړ���̈ʒu
    };

public:
    ChainScoreMoveAnimationComponent(Param param) : param(param) {};
    ~ChainScoreMoveAnimationComponent() {};

    // �J�n�֐�
    void Start()  override {};
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override {};      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override {};

    // ���O�擾
    const char* GetName()const  override { return "ChainScoreMoveAnimationComponent"; };

    // �D��x
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    // �X�R�A�`�F�C���I�����Ɏ��s����֐�
    void OnScoreChainEnd();

private:
    Param param;

private:
    std::weak_ptr<FadeControllerComponent> fade_controller_Wptr;
    std::weak_ptr<SpriteMoverComponent> sprite_mover_Wptr;

#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI()  override {};
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

