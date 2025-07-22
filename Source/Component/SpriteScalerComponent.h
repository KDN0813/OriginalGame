#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include <deque>

class BaseSpriteComponent;
class Transform2DComponent;

// �X�v���C�g�̊g��E�k���A�j���[�V�������s��
class SpriteScalerComponent : public Component
{
public:
    enum class State
    {
        Start = 0,
        Run,
        End
    };

    struct ScaleCommand
    {
        DirectX::XMFLOAT2 target_scale;
        float transition_duration;
    };
public:
    SpriteScalerComponent() : command_pool(){};
    ~SpriteScalerComponent() {};

    // �J�n�֐�
    void Start()  override {};
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override;      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override;

    // ���O�擾
    const char* GetName()const  override { return "SpriteScalerComponente"; };

    // �D��x
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    // ���߂�擪�ɂɒǉ�
    void PushFrontCommand(const ScaleCommand& command);
    void PushFrontCommand(DirectX::XMFLOAT2 target_scale, float transition_duration);
    void PushFrontCommand(float target_scale, float transition_duration);
    // ���߂��Ō���ɒǉ�
    void PushBackCommand(const ScaleCommand& command);
    void PushBackCommand(DirectX::XMFLOAT2 target_scale, float transition_duration);
    void PushBackCommand(float target_scale, float transition_duration);

    // �R�}���h����ɂ���
    void CommandClear();

private:
    std::deque<ScaleCommand> command_pool;

private:
    std::weak_ptr<BaseSpriteComponent> sprite_Wptr;
    std::weak_ptr<Transform2DComponent> transform_Wptr;

    float interpolation_timer = 0.0f;   // ��ԂɎg���^�C�}�[
    State state = State::Start;
    DirectX::XMFLOAT2 start_scale{};    // ��ԊJ�n���̃X�P�[��
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
private:
    float debug_change_scale = 1.0f;
    float debug_transition_duration = 1.0f;

#endif // DEBUG
};

