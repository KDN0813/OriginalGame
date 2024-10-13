#pragma once
#include "Component.h"
#include "StateMachine/State.h"

class StateMachineComponent : public Component
{
public:
    StateMachineComponent() {};
    virtual ~StateMachineComponent() {};

    // �J�n�֐�
    void Start()  override;
    // �I���֐�
    void End()  override;
    // �X�V�֐�
    void Update(float elapsed_time) override;

    // ���O�擾
    const char* GetName()const  override { return "StateMachineComponent"; };

    // �D��x
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::CRITICAL; };

private:
    std::vector<StateBase> state_pool;

#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI()  override {};
#endif // DEBUG
};

