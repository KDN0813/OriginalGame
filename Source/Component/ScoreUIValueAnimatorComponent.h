#pragma once
#include "Component.h"

#include <DirectXMath.h>

class TextNumberValueInterpolatorComponent;
class SpriteScalerComponent;

// �X�R�AUI�̒l���A�j���[�V�����Ǘ�����R���|�[�l���g
class ScoreUIValueAnimatorComponent : public Component
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
        float animetion_total_time;     // �X�R�A�������A�j���[�V�����̑�����
        ScaleParam expanded;  // �g�厞�̃X�P�[���Ɗ���
        ScaleParam shrink;    // �k�����̃X�P�[���Ɗ���
    };
public:
    ScoreUIValueAnimatorComponent(Param param):param(param){};
    ~ScoreUIValueAnimatorComponent() {};

    // �J�n�֐�
    void Start()  override {};
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override {};      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override {};

    // ���O�擾
    const char* GetName()const  override { return "ScoreUIValueAnimatorComponent"; };

    // �D��x
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    // �X�R�A�`�F�C���I�����Ɏ��s����֐�
    void OnScoreChainEnd();
private:
    Param param;

private:
    std::weak_ptr<TextNumberValueInterpolatorComponent> value_interpolator_Wptr;
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
