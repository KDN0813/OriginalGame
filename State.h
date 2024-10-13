#pragma once
#include <vector>
#include <memory>

template <class T>
concept is_Judgement = requires{ std::is_base_of_v<AnimeTransitionJudgementBase, T>; };

class AnimeTransitionJudgementBase;
using StateIndex = size_t;

class StateBase
{
public:
    // ステートの遷移情報
    struct StateTransitionInfo
    {
        StateIndex state_index;
        std::unique_ptr<AnimeTransitionJudgementBase> judgement;	// 遷移判定
    };

    enum class JudgementUpdatePhase
    {
        PreUpdate,      // 更新前の状態
        PostUpdate,     // 更新後の状態
    };
public:
    StateBase(){}
    virtual ~StateBase() {};

    virtual void Start() {};

    // 個々の更新と状態遷移の更新をする
    // 更新関数
    void Update(float elapsed_time) {};

    // 遷移判定
    bool RreTransitionJudgemen() {};
    bool PostTransitionJudgemen() {};

    virtual const char* Name() = 0;

    void AddStateTransition(StateIndex state_index, std::unique_ptr<AnimeTransitionJudgementBase> judgement, JudgementUpdatePhase phase);

private:
    std::vector<StateTransitionInfo> pre_update_judgement_pool;     // Update前に遷移判定を行う
    std::vector<StateTransitionInfo> post_update_judgement_pool;    // Update後に遷移判定を行う
#ifdef _DEBUG
public:
    virtual void DrawDebugGUI() {};

#endif // DEBUG
};
