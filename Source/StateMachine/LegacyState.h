#pragma once
#include <vector>
#include <memory>
#include <string>
#include "System/Alias/OwnerAlias.h"
#include "System/MyHash.h"
#include "StateMachine/LegacyTransitionJudgement.h"

static constexpr size_t INVALID_STATE_INDEX = SIZE_MAX;   // 無効なステートのインデックス(ステートのインデックスの初期値・エラーコードに使用する)
using StateIndex = size_t;

// (旧)ステートの遷移情報
class LegacyStateTransitionInfo
{
public:
public:
    LegacyStateTransitionInfo(std::string next_state_name, std::unique_ptr<TransitionJudgementBase>&& judgement);
    ~LegacyStateTransitionInfo() {};

    StateIndex GetNextStateIndex() { return this->next_state_index; }
    MyHash GetNextStateNameHash() { return this->next_state_name; }
    std::string GetNextStateNameStr() { return this->next_state_name.GetString(); }
    const char* GetNextStateNameCstr() { return this->next_state_name.GetString().c_str(); }
    TransitionJudgementBase* GetJudgement() { return judgement.get(); }
    void SetNextStateIndex(StateIndex index) { this->next_state_index = index; }
private:
    StateIndex next_state_index;                                // 遷移先のステートのインデックス
    MyHash next_state_name;                                // 遷移先のステートの名前
    std::unique_ptr<TransitionJudgementBase> judgement;	// 遷移判定
};

class LegacyStateBase
{
public:
    enum class JudgementUpdatePhase
    {
        PreUpdate,      // 更新前の状態
        PostUpdate,     // 更新後の状態
    };
public:
    LegacyStateBase() = delete;
    LegacyStateBase(std::string name) :name(name) {};
    virtual ~LegacyStateBase() {};

    // 開始関数
    virtual void Start() {};
    // 個々の更新と状態遷移の更新をする
    // 更新関数
    virtual void Update(float elapsed_time) {};
    // 終了関数
    virtual void End() {};

    // 遷移準備が完了しているか
    // 遷移判定クラスで遷移準備を待つ設定の時に使用する
    virtual bool IsTransitionReady() { return true; };

    void SetOwner(OwnerPtr owner);

    const char* GetNameCStr() { return name.GetString().c_str(); };
    std::string GetNameStr() { return name.GetString(); };
    MyHash GetHash() { return name; };

    // 遷移判定のロジックを実行
    // `judgemenのshould_reverse` フラグがtrueなら、遷移判定結果を反転する
    bool PerformTransitionJudgement(TransitionJudgementBase* judgemen);
    
    // 遷移ステートの追加
    void AddStateTransition(std::unique_ptr<LegacyStateTransitionInfo> state_transition, JudgementUpdatePhase phase);
    
    // 各種設定・取得関数
    const std::vector<std::unique_ptr<LegacyStateTransitionInfo>>& GetPreUpdateJudgementPool() { return this->pre_update_judgement_pool; }
    const std::vector<std::unique_ptr<LegacyStateTransitionInfo>>& GetPostUpdateJudgementPool() { return this->post_update_judgement_pool; }
    StateIndex GetStateIndex() { return this->state_index; }
    void SetStateIndex(StateIndex index) { this->state_index = index; }
protected:
    OwnerWPtr owner_Wptr;
private:
    StateIndex state_index = -1;
    MyHash name;
    std::vector<std::unique_ptr<LegacyStateTransitionInfo>> pre_update_judgement_pool;     // Update前に遷移判定を行う
    std::vector<std::unique_ptr<LegacyStateTransitionInfo>> post_update_judgement_pool;    // Update後に遷移判定を行う
#ifdef _DEBUG
public:
    virtual void DrawDebugGUI() {};

#endif // DEBUG
};

template <class T>
concept Legacy_is_State = requires{ std::is_base_of_v<LegacyStateBase, T>; };