#pragma once
#include "Component.h"
#include "StateMachine/State.h"

class StateMachineComponent : public Component
{
public:
    StateMachineComponent();
    virtual ~StateMachineComponent() {};

    // 開始関数
    void Start()  override;
    // 終了関数
    void End()  override;
    // 更新関数
    void Update(float elapsed_time) override;
    // 名前取得
    const char* GetName()const  override { return "StateMachineComponent"; };
    // 優先度
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::CRITICAL; };

    void ChangeState(StateIndex state_index);

    void SetDefaultState(StateIndex state_index);
    void SetDefaultState(const char* state_name);

    // 更新関数の前の遷移判定
    void PreTransitionJudgemen(StateBase* state);
    // 更新関数の後の遷移判定
    void PostTransitionJudgemen(StateBase* state);

    // ステートを名前検索する
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
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()  override ;
#endif // DEBUG
};

