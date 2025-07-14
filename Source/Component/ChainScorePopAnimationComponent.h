#pragma once
#include "Component.h"

#include <DirectXMath.h>

class TextNumberComponent;
class FadeControllerComponent;
class SpriteScalerComponent;
class SpriteMoverComponent;

// �A���X�R�AUI�̏o�����o�Ɗg��E�k���A�j���[�V�����𐧌䂷��R���|�[�l���g
class ChainScorePopAnimationComponent : public Component
{
public:
    struct Param
    {
        float fead_in_time = 1.0f;
        float fead_out_time = 1.0f;

        float expanded_scale = 2.5f;    // �g�厞�̃X�P�[��
        float shrink_scale = 0.5f;      // �k�����̃X�P�[��
        float time_to_expand = 1.0f;    // �g��ɂ����鎞��
        float time_to_shrink = 1.0f;    // �k���ɂ����鎞��

        DirectX::XMFLOAT2 initial_pos = {}; // �����ʒu
        DirectX::XMFLOAT2 target_pos = {};  // �ړ���̈ʒu

        bool is_chain_end_direction = false;   // �A���I�����o�t���O
    };
public:
    ChainScorePopAnimationComponent(Param p) : param(p){};
    ~ChainScorePopAnimationComponent() {};

    // �J�n�֐�
    void Start()  override {};
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override;      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override;

    // ���O�擾
    const char* GetName()const  override { return "ChainScoreUIControllerComponent"; };

    // �D��x
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    // �\������X�R�A�������������Ɏ��s����֐�
    void OnScoreAdded(int);

    // �X�R�A�`�F�C���J�n�Ɏ��s����֐�
    void OnScoreChainStart();
    // �X�R�A�`�F�C���I�����Ɏ��s����֐�
    void OnScoreChainEnd();

private:
    Param param;

private:
    std::weak_ptr<TextNumberComponent> text_number_Wptr;
    std::weak_ptr<FadeControllerComponent> fade_controller_Wptr;
    std::weak_ptr<SpriteScalerComponent> sprite_scaler_Wptr;
    std::weak_ptr<SpriteMoverComponent> sprite_mover_Wptr;

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

