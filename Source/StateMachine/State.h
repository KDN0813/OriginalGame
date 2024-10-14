#pragma once
#include <vector>
#include <memory>
#include <string>
#include "System/Alias/OwnerAlias.h"
#include "Model/AnimeTransitionJudgement.h"

using StateIndex = size_t;

// ステートの遷移情報
class StateTransitionInfo
{
public:
    StateTransitionInfo(std::string next_state_name, std::unique_ptr<AnimeTransitionJudgementBase> judgement);
    ~StateTransitionInfo() {};

    StateIndex GetNextStateIndex() { return this->next_state_index; }
    std::string GetNextStateName() { return this->next_state_name; }
    const char* GetNextStateNameC() { return this->next_state_name.c_str(); }
    AnimeTransitionJudgementBase* GetJudgement() { return judgement.get(); }
    void SetNextStateIndex(StateIndex index) { this->next_state_index = index; }
private:
    StateIndex next_state_index;                                // 遷移先のステートのインデックス
    std::string next_state_name;                                // 遷移先のステートの名前
    std::unique_ptr<AnimeTransitionJudgementBase> judgement;	// 遷移判定
};

class StateBase
{
public:
    enum class JudgementUpdatePhase
    {
        PreUpdate,      // 更新前の状態
        PostUpdate,     // 更新後の状態
    };
public:
    StateBase(){}
    virtual ~StateBase() {};

    // 開始関数
    virtual void Start() {};
    // 個々の更新と状態遷移の更新をする
    // 更新関数
    virtual void Update(float elapsed_time) {};
    // 終了関数
    virtual void End() {};

    virtual const char* Name() = 0;

    // 遷移準備が完了しているか
    // 遷移判定クラスで遷移準備を待つ設定の時に使用する
    virtual bool IsTransitionReady() { return true; };

    void SetOwner(OwnerPtr owner);

    // 遷移判定のロジックを実行
    // `judgemenのshould_reverse` フラグがtrueなら、遷移判定結果を反転する
    bool PerformTransitionJudgement(AnimeTransitionJudgementBase* judgemen);
    
    // 遷移ステートの追加
    void AddStateTransition(std::unique_ptr<StateTransitionInfo> state_transition, JudgementUpdatePhase phase);
    
    // 各種設定・取得関数
    const std::vector<std::unique_ptr<StateTransitionInfo>>& GetPreUpdateJudgementPool() { return this->pre_update_judgement_pool; }
    const std::vector<std::unique_ptr<StateTransitionInfo>>& GetPostUpdateJudgementPool() { return this->post_update_judgement_pool; }
    StateIndex GetStateIndex() { return this->state_index; }
    void SetStateIndex(StateIndex index) { this->state_index = index; }
protected:
    OwnerWPtr owner_Wptr;
private:
    StateIndex state_index = -1;
    std::vector<std::unique_ptr<StateTransitionInfo>> pre_update_judgement_pool;     // Update前に遷移判定を行う
    std::vector<std::unique_ptr<StateTransitionInfo>> post_update_judgement_pool;    // Update後に遷移判定を行う
#ifdef _DEBUG
public:
    virtual void DrawDebugGUI() {};

#endif // DEBUG
};

template <class T>
concept is_State = requires{ std::is_base_of_v<StateBase, T>; };