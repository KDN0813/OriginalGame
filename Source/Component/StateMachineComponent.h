#pragma once
#include "Component.h"
#include "StateMachine/State.h"

class StateMachineComponent : public Component
{
public:
    StateMachineComponent();
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

    void SetDefaultState(StateIndex state_index);
    void SetDefaultState(const char* state_name);

    // �X�V�֐��̑O�̑J�ڔ���
    void PreTransitionJudgemen(StateBase* state);
    // �X�V�֐��̌�̑J�ڔ���
    void PostTransitionJudgemen(StateBase* state);

    // �X�e�[�g�𖼑O��������
    StateBase* FindState(std::string name);
    StateIndex FindStateIndex(std::string name);

    template<is_State State, typename ... Arguments>
    State* RegisterState(Arguments ... args)
    {
        std::unique_ptr<State> state_Uptr = std::make_unique<State>(args...);
        State* state = state_Uptr.get();
        state_pool.emplace_back(std::move(state_Uptr));
        state->SetOwner(GetOwner());
        state->SetStateIndex(state_pool.size() - 1);
        return state;
    }
private:
    std::vector<std::unique_ptr<StateBase>> state_pool;
    StateIndex state_index = 0;
    StateIndex next_state = 0;
    StateIndex default_state = 0;

#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI()  override ;
#endif // DEBUG
};

