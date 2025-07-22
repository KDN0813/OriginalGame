#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include <deque>

class BaseSpriteComponent;
class Transform2DComponent;

// �X�v���C�g�̈ړ��A�j���[�V�������s��
class SpriteMoverComponent : public Component
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
        DirectX::XMFLOAT2 target_pos;
        float transition_duration; // ��Ԏ���
    };
public:
    SpriteMoverComponent() : command_pool() {};
    ~SpriteMoverComponent() {};

    // �J�n�֐�
    void Start()  override {};
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override;      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override;

    // ���O�擾
    const char* GetName()const  override { return "SpriteMoverComponent"; };

    // �D��x
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    // ���߂�擪�ɂɒǉ�
    void PushFrontCommand(const ScaleCommand& command);
    void PushFrontCommand(DirectX::XMFLOAT2 target_scale, float transition_duration);
    // ���߂��Ō���ɒǉ�
    void PushBackCommand(const ScaleCommand& command);
    void PushBackCommand(DirectX::XMFLOAT2 target_scale, float transition_duration);

    // �R�}���h����ɂ���
    void CommandClear();

private:
    std::deque<ScaleCommand> command_pool;

private:
    std::weak_ptr<BaseSpriteComponent> sprite_Wptr;
    std::weak_ptr<Transform2DComponent> transform_Wptr;

    float interpolation_timer = 0.0f;   // ��ԂɎg���^�C�}�[
    State state = State::Start;
    DirectX::XMFLOAT2 start_pos{};    // ��ԊJ�n���̃X�P�[��
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
    DirectX::XMFLOAT2 debug_change_pos = {};
    float debug_transition_duration = 1.0f;

#endif // DEBUG
};