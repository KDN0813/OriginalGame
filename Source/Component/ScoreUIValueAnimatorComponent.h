#pragma once
#include "Component.h"

class TextNumberValueInterpolatorComponent;

// スコアUIの値をアニメーション管理するコンポーネント
class ScoreUIValueAnimatorComponent : public Component
{
public:
    struct Param
    {
        float animetion_time;   // 
    };
public:
    ScoreUIValueAnimatorComponent(Param param):param(param){};
    ~ScoreUIValueAnimatorComponent() {};

    // 開始関数
    void Start()  override {};
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override {};      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override {};

    // 名前取得
    const char* GetName()const  override { return "ScoreUIValueAnimatorComponent"; };

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    // スコアチェイン終了時に実行する関数
    void OnScoreChainEnd();
private:
    Param param;

private:
    std::weak_ptr<TextNumberValueInterpolatorComponent> value_interpolator_Wptr;

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
