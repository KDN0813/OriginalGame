#pragma once
#include "Component.h"

class TextNumberValueInterpolatorComponent;

// �X�R�AUI�̒l���A�j���[�V�����Ǘ�����R���|�[�l���g
class ScoreUIValueAnimatorComponent : public Component
{
public:
    struct Param
    {
        float animetion_time;   // 
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
