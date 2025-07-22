#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include <deque>

class BaseSpriteComponent;
class Transform2DComponent;

// �X�v���C�g�̐U���A�j���[�V�������s��
class SpriteShakeComponent : public Component
{
public:
    enum class State
    {
        Start = 0,
        Run,
        End
    };

    struct ShakeCommand
    {
        DirectX::XMFLOAT2 force = {};           // ���͂����V�F�C�N�̋���
        DirectX::XMFLOAT2 shake_movement = {};  // �V�F�C�N��
        float end_time = 0.0f;                  
    };
public:
    SpriteShakeComponent() : command_pool() {};
    ~SpriteShakeComponent() {};

    // �J�n�֐�
    void Start()  override {};
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override;      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override;

    // ���O�擾
    const char* GetName()const  override { return "SpriteShakeComponent"; };

    // �D��x
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    // ���߂�擪�ɂɒǉ�
    void PushFrontCommand(const ShakeCommand& command);
    void PushFrontCommand(DirectX::XMFLOAT2 force, DirectX::XMFLOAT2 shake_movement, float shake_time);
    void PushFrontCommand(float target_scale, float transition_duration);
    // ���߂��Ō���ɒǉ�
    void PushBackCommand(const ShakeCommand& command);
    void PushBackCommand(DirectX::XMFLOAT2 force, DirectX::XMFLOAT2 shake_movement, float shake_time);
    void PushBackCommand(float target_scale, float transition_duration);

    // �R�}���h����ɂ���
    void CommandClear();

private:
    std::deque<ShakeCommand> command_pool;
    float tiemr = 0.0f;

    State state = State::Start;
private:
    std::weak_ptr<BaseSpriteComponent> sprite_Wptr;
    std::weak_ptr<Transform2DComponent> transform_Wptr;

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

