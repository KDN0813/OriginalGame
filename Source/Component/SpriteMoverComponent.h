#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include <deque>

class BaseSpriteComponent;
class Transform2DComponent;

// スプライトの移動アニメーションを行う
class SpriteMoverComponent : public Component
{
public:
    enum class State
    {
        Start = 0,
        Run,
        End
    };

    struct ScaleCommand
    {
        DirectX::XMFLOAT2 target_pos;
        float transition_duration; // 補間時間
    };
public:
    SpriteMoverComponent() : command_pool() {};
    ~SpriteMoverComponent() {};

    // 開始関数
    void Start()  override {};
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override;      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "SpriteMoverComponent"; };

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    // 命令を先頭にに追加
    void PushFrontCommand(const ScaleCommand& command);
    void PushFrontCommand(DirectX::XMFLOAT2 target_scale, float transition_duration);
    // 命令を最後尾に追加
    void PushBackCommand(const ScaleCommand& command);
    void PushBackCommand(DirectX::XMFLOAT2 target_scale, float transition_duration);

    // コマンドを空にする
    void CommandClear();

private:
    std::deque<ScaleCommand> command_pool;

private:
    std::weak_ptr<BaseSpriteComponent> sprite_Wptr;
    std::weak_ptr<Transform2DComponent> transform_Wptr;

    float interpolation_timer = 0.0f;   // 補間に使うタイマー
    State state = State::Start;
    DirectX::XMFLOAT2 start_pos{};    // 補間開始時のスケール
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
    DirectX::XMFLOAT2 debug_change_pos = {};
    float debug_transition_duration = 1.0f;

#endif // DEBUG
};