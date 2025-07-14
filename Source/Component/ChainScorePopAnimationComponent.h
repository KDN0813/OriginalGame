#pragma once
#include "Component.h"

#include <DirectXMath.h>
#include <functional>

class TextNumberComponent;
class FadeControllerComponent;
class SpriteScalerComponent;

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

    void SetOnScoreAdded(const std::function<void(int)>& function) { this->on_score_added = function; };
    void SetOnScoreChainStart(const std::function<void()>& function) { this->on_score_chain_start = function; };
    void SetOnScoreChainEnd(const std::function<void()>& function) { this->on_score_chain_end = function; };
private:
    Param param;

    std::function<void(int)> on_score_added;        // �A���X�R�A�����Z���ꂽ���̃R�[���o�b�N�ϐ�
    std::function<void()> on_score_chain_start;     // �A�����J�n�������̃R�[���o�b�N�ϐ�
    std::function<void()> on_score_chain_end;       // �A�����I���������̃R�[���o�b�N�ϐ�
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

