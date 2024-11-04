#pragma once
#include <memory>
#include "LegacyState.h"

class MovementComponent;
class EnemyComponent;
class CircleCollisionComponent;

class Legacy_DefaultState : public LegacyStateBase
{
public:
    Legacy_DefaultState() :LegacyStateBase("DefaultState") {}
    ~Legacy_DefaultState() override {};

    // �J�n�֐�
    void Start() override {};
    // �X�V�֐�
    void Update(float elapsed_time) override {};
    // �I���֐�
    void End() override {};

    // �J�ڏ������������Ă��邩
    // �J�ڔ���N���X�őJ�ڏ�����҂ݒ�̎��Ɏg�p����
    bool IsTransitionReady() override { return true; };
#ifdef _DEBUG
public:
    void DrawDebugGUI() override{};

#endif // DEBUG
};

class Legacy_IdelState : public LegacyStateBase
{
public:
    Legacy_IdelState() :LegacyStateBase("IdelState") {}
    ~Legacy_IdelState() override {};

    void Start() override;

    // �X�V�֐�
    void Update(float elapsed_time) override {};

    void End()override;

    // �J�ڏ������������Ă��邩
    // �J�ڔ���N���X�őJ�ڏ�����҂ݒ�̎��Ɏg�p����
    bool IsTransitionReady() override { return true; };
private:
    std::weak_ptr<EnemyComponent> enemy_Wptr;

#ifdef _DEBUG
public:
    void DrawDebugGUI() override {};

#endif // DEBUG
};

class Legacy_WanderState : public LegacyStateBase
{
public:
    Legacy_WanderState() :LegacyStateBase("WanderState") {}
    ~Legacy_WanderState() override {};

    // �J�n�֐�
    void Start() override;
    // �X�̍X�V�Ə�ԑJ�ڂ̍X�V������
    // �X�V�֐�
    void Update(float elapsed_time) override {};
    // �I���֐�
    void End() override;

    // �J�ڏ������������Ă��邩
    // �J�ڔ���N���X�őJ�ڏ�����҂ݒ�̎��Ɏg�p����
    bool IsTransitionReady() override { return true; };
private:
    std::weak_ptr<EnemyComponent> enemy_Wptr;

#ifdef _DEBUG
public:
    void DrawDebugGUI() override {};

#endif // DEBUG
};

class Legacy_AttackState : public LegacyStateBase
{
public:
    Legacy_AttackState() :LegacyStateBase("AttackState") {}
    ~Legacy_AttackState() override {};

    // �J�n�֐�
    void Start() override;
    // �X�V�֐�
    void Update(float elapsed_time) override;
    // �I���֐�
    void End() override;

    // �J�ڏ������������Ă��邩
    // �J�ڔ���N���X�őJ�ڏ�����҂ݒ�̎��Ɏg�p����
    bool IsTransitionReady() override { return true; };
private:
    std::weak_ptr<CircleCollisionComponent> circle_collision_Wptr;

#ifdef _DEBUG
public:
    void DrawDebugGUI() override {};

#endif // DEBUG
};