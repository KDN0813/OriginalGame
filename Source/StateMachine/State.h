#pragma once
#include <vector>
#include <memory>

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

    virtual const char* Name() = 0;

    // 更新関数の前の遷移判定
    void PreTransitionJudgemen();
    // 更新関数の後の遷移判定
    void PostTransitionJudgemen();

    // 遷移判定のロジックを実行
    // `judgemenのshould_reverse` フラグがtrueなら、遷移判定結果を反転する
    bool PerformTransitionJudgement(AnimeTransitionJudgementBase* judgemen);
    
    // 遷移ステートの追加
    void AddStateTransition(StateIndex state_index, std::unique_ptr<AnimeTransitionJudgementBase> judgement, JudgementUpdatePhase phase);
private:
    std::vector<StateTransitionInfo> pre_update_judgement_pool;     // Update前に遷移判定を行う
    std::vector<StateTransitionInfo> post_update_judgement_pool;    // Update後に遷移判定を行う
#ifdef _DEBUG
public:
    virtual void DrawDebugGUI() {};

#endif // DEBUG
};

template <class T>
concept is_State = requires{ std::is_base_of_v<StateBase, T>; };