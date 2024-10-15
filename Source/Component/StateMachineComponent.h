#pragma once
#include "Component.h"
#include "StateMachine/State.h"
#include "System/MyHash.h"

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
    void SetDefaultState(std::string state_name);
    void SetDefaultState(MyHash state_name);

    // 更新関数の前の遷移判定
    void PreTransitionJudgemen(StateBase* state);
    // 更新関数の後の遷移判定
    void PostTransitionJudgemen(StateBase* state);

    // ステートを名前検索する
    StateBase* FindState(MyHash name);
    StateIndex FindStateIndex(MyHash name);

    template<is_State State, typename ... Arguments>
    State* RegisterState(Arguments ... args)
    {
        std::unique_ptr<State> state_Uptr = std::make_unique<State>(args...);
        State* state = state_Uptr.get();
        state_pool.emplace_back(std::move(state_Uptr));
        state->SetOwner(GetOwner());
        state->SetStateIndex(state_pool.size() - 1);
#ifdef _DEBUG
        state_name_pool.emplace_back(state->GetNameStr());
#endif // _DEBUG
        return state;
    }
private:
    std::vector<std::unique_ptr<StateBase>> state_pool;
    StateIndex state_index = INVALID_STATE_INDEX;
    StateIndex next_state = INVALID_STATE_INDEX;
    StateIndex default_state = INVALID_STATE_INDEX;

#ifdef _DEBUG
public:
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()  override ;
private:
    std::vector<std::string> state_name_pool;
#endif // DEBUG
};

