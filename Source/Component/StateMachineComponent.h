#pragma once
#include <vector>
#include "Component.h"
#include "StateMachine/StateBase.h"

template <class T>
concept is_State = requires{ std::is_base_of_v<State, T>; };

// ステートマシン
// 内部で条件を保持し、独立して状態遷移を行う
class StateMachineComponent : public Component
{
public:
    StateMachineComponent();
    ~StateMachineComponent() {};

    // 開始関数
    void Start()  override;
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override;      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override;
    // 名前取得
    const char* GetName()const  override { return "StateMachine"; };
    // 優先度
    const COMPONENT_PRIORITY GetPriority()const noexcept  override { return COMPONENT_PRIORITY::CRITICAL; };

    // ステート変更
    void ChangeState(State::ChangeState& chage_state);
    // デフォルトステートの設定
    void SetDefaultState(std::string default_name);

    // ステートを名前検索する
    State* FindState(MyHash name);
    // ステートのインデックスを名前検索する
    State::StateIndex FindStateIndex(MyHash name);
    
    // ステート登録
    template<is_State State>
    void RegisterState()
    {
        std::unique_ptr<State> state = std::make_unique<State>();
        state->SetOwner(GetOwner());
        state->SetStateIndex(this->state_pool.size());
#ifdef _DEBUG
        this->state_name_pool.emplace_back(state->GetHash().GetString());
#endif // _DEBUG
        this->state_pool.emplace_back(std::move(state));
    };
private:
    State* current_state = nullptr;                     // 現在のステート
    std::vector<std::unique_ptr<State>> state_pool;     // 各ステートを保持する配列
    State::ChangeState default_state;

#ifdef _DEBUG
public:
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()  override;
    /**
     * デバックの情報を3D画面上に出力する関数
     */
    void DrawDebugPrimitive()  override {};
    /**
     * デバッグプリミティブ表示用ImGui
     */
    void DrawDebugPrimitiveGUI()  override {};
    bool IsDebugPrimitive() override { return false; }   // DebugPrimitiveが存在するか
private:
    std::vector<std::string> state_name_pool;                   // ステートの名前の配列
#endif // DEBUG
};

