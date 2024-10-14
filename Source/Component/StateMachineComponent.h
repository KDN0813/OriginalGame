#pragma once
#include "Component.h"
#include "StateMachine/State.h"

class StateMachineComponent : public Component
{
public:
    StateMachineComponent(StateIndex state_max);
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

    // 更新関数の前の遷移判定
    void PreTransitionJudgemen(StateBase* state);
    // 更新関数の後の遷移判定
    void PostTransitionJudgemen(StateBase* state);

    // ステートを名前検索する
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
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()  override {};
#endif // DEBUG
};

