#pragma once
#include "Component.h"

#include <DirectXMath.h>

class TextNumberValueInterpolatorComponent;
class SpriteScalerComponent;

// スコアUIの値をアニメーション管理するコンポーネント
class ScoreUIValueAnimatorComponent : public Component
{
public:
    // スケーリングのパラメータ
    struct ScaleParam
    {
        DirectX::XMFLOAT2 target_scale;  // 目的のスケール
        float ratio;                     // 拡大・縮小に使う割合（合計1.0になるように設定）
    };
    struct Param
    {
        float animetion_total_time;     // スコアが増加アニメーションの総時間
        ScaleParam expanded;  // 拡大時のスケールと割合
        ScaleParam shrink;    // 縮小時のスケールと割合
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
    std::weak_ptr<SpriteScalerComponent> sprite_scaler_Wptr;

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
