#pragma once
#include <memory>
#include "LegacyState.h"

class MovementComponent;
class EnemyComponent;
class CircleCollisionComponent;

class DefaultState : public LegacyStateBase
{
public:
    DefaultState() :LegacyStateBase("DefaultState") {}
    ~DefaultState() override {};

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

class IdelState : public LegacyStateBase
{
public:
    IdelState() :LegacyStateBase("IdelState") {}
    ~IdelState() override {};

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

class WanderState : public LegacyStateBase
{
public:
    WanderState() :LegacyStateBase("WanderState") {}
    ~WanderState() override {};

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

class AttackState : public LegacyStateBase
{
public:
    AttackState() :LegacyStateBase("AttackState") {}
    ~AttackState() override {};

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