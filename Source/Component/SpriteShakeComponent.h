#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include <deque>

class BaseSpriteComponent;
class Transform2DComponent;

// スプライトの振動アニメーションを行う
class SpriteShakeComponent : public Component
{
public:
    enum class State
    {
        Start = 0,
        Run,
        End
    };

    struct ShakeCommand
    {
        DirectX::XMFLOAT2 force = {};           // 入力されるシェイクの強さ
        DirectX::XMFLOAT2 shake_movement = {};  // シェイク量
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
    void PushFrontCommand(DirectX::XMFLOAT2 force, DirectX::XMFLOAT2 shake_movement, float shake_time);
    void PushFrontCommand(float target_scale, float transition_duration);
    // 命令を最後尾に追加
    void PushBackCommand(const ShakeCommand& command);
    void PushBackCommand(DirectX::XMFLOAT2 force, DirectX::XMFLOAT2 shake_movement, float shake_time);
    void PushBackCommand(float target_scale, float transition_duration);

    // コマンドを空にする
    void CommandClear();

private:
    std::deque<ShakeCommand> command_pool;
    float tiemr = 0.0f;

    State state = State::Start;
private:
    std::weak_ptr<BaseSpriteComponent> sprite_Wptr;
    std::weak_ptr<Transform2DComponent> transform_Wptr;

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
    float debug_change_scale = 1.0f;
    float debug_transition_duration = 1.0f;

#endif // DEBUG
};

