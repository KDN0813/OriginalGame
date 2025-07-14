#pragma once
#include "Component.h"
#include <DirectXMath.h>

class FadeControllerComponent;
class SpriteMoverComponent;

// 連鎖スコアUIを合計スコアへ移動させるアニメーションを制御するコンポーネント
class ChainScoreMoveAnimationComponent : public Component
{
public:
    struct Param
    {
        float fead_out_time = 1.0f;
        DirectX::XMFLOAT2 initial_pos = {}; // 初期位置
        DirectX::XMFLOAT2 target_pos = {};  // 移動先の位置
    };

public:
    ChainScoreMoveAnimationComponent(Param param) : param(param) {};
    ~ChainScoreMoveAnimationComponent() {};

    // 開始関数
    void Start()  override {};
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override {};      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override {};

    // 名前取得
    const char* GetName()const  override { return "ChainScoreMoveAnimationComponent"; };

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    // スコアチェイン終了時に実行する関数
    void OnScoreChainEnd();

private:
    Param param;

private:
    std::weak_ptr<FadeControllerComponent> fade_controller_Wptr;
    std::weak_ptr<SpriteMoverComponent> sprite_mover_Wptr;

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

