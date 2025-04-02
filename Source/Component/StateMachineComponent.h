#pragma once
#include <unordered_map>
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
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::CRITICAL; };

    // ステート変更
    void ChangeState(std::string state_key);
    // デフォルトステートの設定
    void SetDefaultState(std::string state_key);

    // ステートを名前検索する
    State* FindState(std::string state_key);
    
    // ステート登録
    template<is_State State>
    void RegisterState(std::string state_key)
    {
#ifdef _DEBUG
        this->state_name_pool.emplace_back(state_key);
#endif // _DEBUG
        this->state_pool[state_key] = std::make_unique<State>();
        this->state_pool[state_key]->SetOwner(GetOwner());
    };
private:
    State* current_state = nullptr;                                         // 現在のステート
    std::unordered_map<std::string, std::unique_ptr<State>> state_pool;     // 各ステートを保持する配列
    std::string default_state_key;

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

