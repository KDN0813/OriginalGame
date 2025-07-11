#pragma once
#include "Component.h"
#include <functional>

// 連続撃破数を制御するコンポーネント
class ChainKillCounterComponent : public Component
{
public:
    struct Param
    {
        int chain_kill_count = 0;           // 連続撃破数
        float chain_kill_timer = 0.0f;      // 連続撃破の残り猶予時間
        float chain_kill_timer_max = 0.0f;  // 連続撃破の猶予時間の最大値
        bool is_reset = false;              // カウントリセットが1度だけ実行されるよう制御するフラグ
    };
public:
    ChainKillCounterComponent(Param param) : param(param) {};
    ~ChainKillCounterComponent() {};

    // 開始関数
    void Start()  override {};
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override;      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "ChainKillCounterComponent"; };

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    void CountReset();

    void SetOnKillCountAdded(const std::function<void(int)>& function) { this->onKillCountAdded = function; };  // キルカウントの増価
    void SetOnChainKillEnded(const std::function<void()>& function) { this->onKillCountReset = function; };     // キルカウント終了

    // 連続撃破数を加算する
    void AddChainKill();
private:
    Param param;

    std::function<void(int)> onKillCountAdded;
    std::function<void()> onKillCountReset;
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
#endif // DEBUG
};

