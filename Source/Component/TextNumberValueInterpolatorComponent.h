#pragma once
#include "Component.h"
#include <deque>

class TextNumberComponent;

// TextNumberComponent�̒l���Ԃ���R���|�[�l���g
class TextNumberValueInterpolatorComponent : public Component
{
public:
    enum class State
    {
        Start = 0,
        Run,
        End
    };

    struct Command
    {
        int target_value;           // ��Ԍ�̒l
        float transition_duration;  // ��Ԏ���
    };

public:
    TextNumberValueInterpolatorComponent()
        :command_pool()
        , state(State::Start)
        , interpolation_timer()
        , start_value()
    {};
    ~TextNumberValueInterpolatorComponent() {};

    // �J�n�֐�
    void Start()  override {};
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override;      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override;

    // ���O�擾
    const char* GetName()const  override { return "TextNumberValueInterpolatorComponent"; };

    // �D��x
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    // �R�}���h����ɂ���
    void CommandClear();

    void PushFrontCommand(const Command& command);
    void PushFrontCommand(int target_value, float transition_duration);
    // ���߂��Ō���ɒǉ�
    void PushBackCommand(const Command& command);
    void PushBackCommand(int target_value, float transition_duration);
private:
    std::deque<Command> command_pool;
    State state;
    float interpolation_timer;   // ��ԂɎg���^�C�}�[
    float start_value;

private:
    std::weak_ptr<TextNumberComponent> text_number_Wptr;

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

