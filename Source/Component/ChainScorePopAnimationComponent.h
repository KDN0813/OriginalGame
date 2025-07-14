#pragma once
#include "Component.h"

#include <DirectXMath.h>

class TextNumberComponent;
class FadeControllerComponent;
class SpriteScalerComponent;

// 連鎖スコアUIの出現演出と拡大・縮小アニメーションを制御するコンポーネント
class ChainScorePopAnimationComponent : public Component
{
public:
    // スケーリングのパラメータ
    struct ScaleParam
    {
        DirectX::XMFLOAT2 target_scale;  // 目的のスケール
        float time;          // 目的のスケールになるまでの時間
    };

    struct Param
    {
        float fead_in_time = 1.0f;

        ScaleParam expanded;            // 拡大時のパラメータ
        ScaleParam shrink;              // 縮小時のパラメータ
    };
public:
    ChainScorePopAnimationComponent(Param p) : param(p){};
    ~ChainScorePopAnimationComponent() {};

    // 開始関数
    void Start()  override {};
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override;      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "ChainScorePopAnimationComponent"; };

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    // 表示するスコアが増加した時に実行する関数
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

