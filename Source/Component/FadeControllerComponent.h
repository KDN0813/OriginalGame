#pragma once
#include "Component.h"

class BaseSpriteComponent;

enum class FEAD_TYPE
{
    FEAD_IN = 0,
    FEAD_OUT,

    MAX,
};

// �X�v���C�g�̃t�F�[�h�A�j���[�V�������s��
class FadeControllerComponent : public Component
{
public:
    enum class FADE_STATE
    {
        START = 0,
        RUN,
        END,
    };

    struct FadeControllerParam
    {
        float fade_duration = 0.2f; // �t�F�[�h�̌p������
        float fade_time = 0.0f;     // �t�F�[�h�̎c�莞��
        FEAD_TYPE fead_type = FEAD_TYPE::FEAD_IN;
        FADE_STATE state = FADE_STATE::START;
    };
public:
    FadeControllerComponent() = delete;
    FadeControllerComponent(FadeControllerParam param) :param(param){};
    ~FadeControllerComponent() {};

    // �J�n�֐�
    void Start()  override;
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override { this->param.state = FADE_STATE::START; this->SetIsActive(false); };      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override;

    // ���O�擾
    const char* GetName()const  override { return "FadeControllerComponent"; };

    // �D��x
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    /**
     * �t�F�[�h���J�n����
     * ���g���A�N�e�B�u��������
     */
    void FeadStart(FEAD_TYPE type, float fade_duration);
    
    // alpha�l��ݒ肷��
    void SetAlpha(float);

    FADE_STATE GetFeadState() { return this->param.state; }
private:
    void FeadIn();
    void FeadOut();

private:
    FadeControllerParam param;

private:
    std::weak_ptr<BaseSpriteComponent> sprite_Wptr;

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

