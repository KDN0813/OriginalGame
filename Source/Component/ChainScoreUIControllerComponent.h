#pragma once
#include "Component.h"

class TextNumberComponent;
class FadeControllerComponent;

// �A���X�R�A�擾��UI�𐧌䂷��R���|�[�l���g
class ChainScoreUIControllerComponent : public Component
{
public:
    struct Param
    {
        float fead_in_time = 1.0f;
        float fead_out_time = 1.0f;
        bool is_chain_end_direction = false;   // �A���I�����o�t���O
    };
public:
    ChainScoreUIControllerComponent(Param p) : param(p){};
    ~ChainScoreUIControllerComponent() {};

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

