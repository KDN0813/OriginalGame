#pragma once
#include "State.h"


class DefaultState : public StateBase
{
public:
    DefaultState() {}
    ~DefaultState() override {};

    void Start() override {};

    // �X�V�֐�
    void Update(float elapsed_time) override {};

    const char* Name() override { return "DefaultState"; };

    // �J�ڏ������������Ă��邩
    // �J�ڔ���N���X�őJ�ڏ�����҂ݒ�̎��Ɏg�p����
    bool IsTransitionReady() override { return true; };
#ifdef _DEBUG
public:
    void DrawDebugGUI() override{};

#endif // DEBUG
};

class IdelState : public StateBase
{
public:
    IdelState() {}
    ~IdelState() override {};

    void Start() override {};

    // �X�V�֐�
    void Update(float elapsed_time) override {};

    const char* Name() override { return "IdelState"; };

    // �J�ڏ������������Ă��邩
    // �J�ڔ���N���X�őJ�ڏ�����҂ݒ�̎��Ɏg�p����
    bool IsTransitionReady() override { return true; };
private:
    float state_timer = 0.0f;

#ifdef _DEBUG
public:
    void DrawDebugGUI() override {};

#endif // DEBUG
};

class WanderState : public StateBase
{
public:
    WanderState() {}
    ~WanderState() override {};

    void Start() override {};

    // �X�̍X�V�Ə�ԑJ�ڂ̍X�V������
    // �X�V�֐�
    void Update(float elapsed_time) override {};

    const char* Name() override { return "WanderState"; };

    // �J�ڏ������������Ă��邩
    // �J�ڔ���N���X�őJ�ڏ�����҂ݒ�̎��Ɏg�p����
    bool IsTransitionReady() override { return true; };
#ifdef _DEBUG
public:
    void DrawDebugGUI() override {};

#endif // DEBUG
};