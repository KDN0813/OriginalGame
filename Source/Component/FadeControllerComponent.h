#pragma once
#include "Component.h"

class BaseSpriteComponent;

enum class FEAD_TYPE
{
    FEAD_IN = 0,
    FEAD_OUT,

    MAX,
};

// スプライトのフェードアニメーションを行う
class FadeControllerComponent : public Component
{
public:
    enum class FADE_STATE
    {
        START = 0,
        RUN,
        END,
    };

    struct FadeControllerParam
    {
        float fade_duration = 0.2f; // フェードの継続時間
        float fade_time = 0.0f;     // フェードの残り時間
        FEAD_TYPE fead_type = FEAD_TYPE::FEAD_IN;
        FADE_STATE state = FADE_STATE::START;
    };
public:
    FadeControllerComponent() = delete;
    FadeControllerComponent(FadeControllerParam param) :param(param){};
    ~FadeControllerComponent() {};

    // 開始関数
    void Start()  override;
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override { this->param.state = FADE_STATE::START; this->SetIsActive(false); };      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "FadeControllerComponent"; };

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    /**
     * フェードを開始する
     * 自身をアクティブ化させる
     */
    void FeadStart(FEAD_TYPE type, float fade_duration);
    
    // alpha値を設定する
    void SetAlpha(float);

    FADE_STATE GetFeadState() { return this->param.state; }
private:
    void FeadIn();
    void FeadOut();

private:
    FadeControllerParam param;

private:
    std::weak_ptr<BaseSpriteComponent> sprite_Wptr;

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

