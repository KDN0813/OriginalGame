#pragma once
#include "Component.h"

// 連続撃破数のUIを制御するコンポーネント
class ChainKillCounterUIControllerComponent : public Component
{
public:
    struct Param
    {
        float chain_kill_timer;     // 連続撃破の残り猶予時間
        float chain_kill_timer_max; // 連続撃破の猶予時間の最大値
    };
public:
    ChainKillCounterUIControllerComponent(Param param) : param(param) {};
    ~ChainKillCounterUIControllerComponent() {};

    // 開始関数
    void Start()  override {};
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override;      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override {};

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

private:
    Param param;

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

