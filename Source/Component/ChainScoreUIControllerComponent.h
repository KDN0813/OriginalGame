#pragma once
#include "Component.h"

class TextNumberComponent;
class FadeControllerComponent;

// 連続スコア取得のUIを制御するコンポーネント
class ChainScoreUIControllerComponent : public Component
{
public:
    struct Param
    {
        float fead_in_time = 1.0f;
        float fead_out_time = 1.0f;
        bool is_chain_end_direction = false;   // 連鎖終了演出フラグ
    };
public:
    ChainScoreUIControllerComponent(Param p) : param(p){};
    ~ChainScoreUIControllerComponent() {};

    // 開始関数
    void Start()  override {};
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override;      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "ChainScoreUIControllerComponent"; };

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    // 表示するスコアが増価した時に実行する関数
    void OnScoreAdded(int);

    // スコアチェイン開始に実行する関数
    void OnScoreChainStart();
    // スコアチェイン終了時に実行する関数
    void OnScoreChainEnd();

private:
    Param param;

private:
    std::weak_ptr<TextNumberComponent> text_number_Wptr;
    std::weak_ptr<FadeControllerComponent> fade_controller_Wptr;

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

