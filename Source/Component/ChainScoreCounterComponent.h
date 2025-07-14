#pragma once
#include "Component.h"
#include <functional>

// 連続取得スコアを制御するコンポーネント
class ChainScoreCounterComponent : public Component
{
public:
    struct Param
    {
        int chain_score = 0;           
        float chain_timer = 0.0f;      
        float chain_timer_max = 0.0f;  
    };
public:
    ChainScoreCounterComponent(Param param) : param(param) {};
    ~ChainScoreCounterComponent() {};

    // 開始関数
    void Start()  override;
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override;      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "ChainScoreCounterComponent"; };

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    void ChainEnd();

    void SetOnScoreAdded(const std::function<void(int)>& function) { this->OnScoreAdded = function; };   
    void SetOnScoreChainStart(const std::function<void()>& function) { this->OnScoreChainStart = function; };
    void SetOnScoreChainEnd(const std::function<void()>& function) { this->OnScoreChainEnd = function; };

    // スコアを加算する
    void AddChain(int add_score);
private:
    Param param;

    std::function<void(int)> OnScoreAdded;
    std::function<void()> OnScoreChainStart;
    std::function<void()> OnScoreChainEnd;
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

