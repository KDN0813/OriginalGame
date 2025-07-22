#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include <deque>

class BaseSpriteComponent;

// スプライトの振動アニメーションを行う
class SpriteShakeComponent : public Component
{
public:
    struct ShakeCommand
    {
        DirectX::XMFLOAT2 force = {};           // 入力されるシェイクの強さ
        float end_time = 0.0f;                  
    };
public:
    SpriteShakeComponent() : command_pool() {};
    ~SpriteShakeComponent() {};

    // 開始関数
    void Start()  override {};
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override;      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "SpriteShakeComponent"; };

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    // 命令を先頭にに追加
    void PushFrontCommand(const ShakeCommand& command);
    void PushFrontCommand(DirectX::XMFLOAT2 force, float shake_time);
    // 命令を最後尾に追加
    void PushBackCommand(const ShakeCommand& command);
    void PushBackCommand(DirectX::XMFLOAT2 force, float shake_time);

private:
    std::deque<ShakeCommand> command_pool;
    DirectX::XMFLOAT2 shake_movement = {};           //  シェイク量
    float tiemr = 0.0f;
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
private:
    // デバッグ時にシェイクさせるための値
    DirectX::XMFLOAT2 debug_force = {};
    float debug_end_time = 0.0f;

#endif // DEBUG
};

