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

    void ChangeState(StateIndex state_index);

    // �X�V�֐��̑O�̑J�ڔ���
    void PreTransitionJudgemen(StateBase* state);
    // �X�V�֐��̌�̑J�ڔ���
    void PostTransitionJudgemen(StateBase* state);

    // �X�e�[�g�𖼑O��������
    StateBase* FindState(std::string name);

    template<is_State State, typename ... Arguments>
    State* RegisterState(Arguments ... args)
    {
        State* state = state_pool.emplace_back(std::make_unique<State>(args));
        state->SetOwner(owner.lock());
        state->SetStateIndex(state_pool.size() - 1);
        return state;
    }
private:
    std::vector<std::unique_ptr<StateBase>> state_pool;
    StateIndex state_index = -1;
    StateIndex next_state = -1;

#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI()  override {};
#endif // DEBUG
};

