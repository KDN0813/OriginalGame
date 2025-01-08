#pragma once
#include "Component.h"

class FadeControllerComponent : public Component
{
public:
    struct FadeControllerParam
    {
        float fade_duration = 0.2f; // フェードの継続時間
        float fade_time = 0.0f;     // フェードの残り時間
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
    void ReStart() override {};      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "FadeControllerComponent"; };

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    void SetFead();
private:
    FadeControllerParam param;

#ifdef _DEBUG
public:
    /**
     * デバックの情報を2D画面に出力する関数
     */
    void DrawDebugGUI()  override {};
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

