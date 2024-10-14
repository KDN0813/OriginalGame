#pragma once
#include "Component.h"
#include "StateMachine/State.h"

class StateMachineComponent : public Component
{
public:
    StateMachineComponent(StateIndex state_max);
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

    template<is_State State, typename ... Arguments>
    State* AddState(Arguments ... args)
    {
        return state_pool.emplace_back(std::make_unique<State>(args));
    }
private:
    std::vector<std::unique_ptr<StateBase>> state_pool;
    StateIndex state = -1;

#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI()  override {};
#endif // DEBUG
};

