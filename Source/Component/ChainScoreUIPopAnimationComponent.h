#pragma once
#include "Component.h"

#include <DirectXMath.h>

class TextNumberComponent;
class FadeControllerComponent;
class SpriteScalerComponent;

// �A���X�R�AUI�̏o�����o�Ɗg��E�k���A�j���[�V�����𐧌䂷��R���|�[�l���g
class ChainScorePopAnimationComponent : public Component
{
public:
    // �X�P�[�����O�̃p�����[�^
    struct ScaleParam
    {
        DirectX::XMFLOAT2 target_scale;  // �ړI�̃X�P�[��
        float ratio;                     // �g��E�k���Ɏg�������i���v1.0�ɂȂ�悤�ɐݒ�j
    };

    struct Param
    {
        float fead_in_time = 1.0f;                  // �t�F�[�h�C���ɂ����鎞��
        float scale_transition_total_time = 1.0f;   // �g�偨�k���̑S�̂̏��v����

        ScaleParam expanded;  // �g�厞�̃X�P�[���Ɗ���
        ScaleParam shrink;    // �k�����̃X�P�[���Ɗ���
    };
public:
    ChainScorePopAnimationComponent(Param p) : param(p){};
    ~ChainScorePopAnimationComponent() {};

    // �J�n�֐�
    void Start()  override {};
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override {};
    // �X�V�֐�
    void Update(float elapsed_time) override {};

    // ���O�擾
    const char* GetName()const  override { return "ChainScorePopAnimationComponent"; };

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

