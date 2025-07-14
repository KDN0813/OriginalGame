#pragma once
#include "Component.h"
#include <deque>

class TextNumberComponent;

// TextNumberComponentの値を補間するコンポーネント
class TextNumberValueInterpolatorComponent : public Component
{
public:
    enum class State
    {
        Start = 0,
        Run,
        End
    };

    struct Command
    {
        int target_value;           // 補間語の値
        float transition_duration;  // 補間時間
    };

public:
    TextNumberValueInterpolatorComponent()
        :command_pool()
        , state(State::Start)
        , interpolation_timer()
        , start_value()
    {};
    ~TextNumberValueInterpolatorComponent() {};

    // 開始関数
    void Start()  override {};
    // 終了関数
    void End()  override {};
    // リスタート処理
    void ReStart() override;      // パラメータの初期化
    // 更新関数
    void Update(float elapsed_time) override;

    // 名前取得
    const char* GetName()const  override { return "TextNumberValueInterpolatorComponent"; };

    // 優先度
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    // コマンドを空にする
    void CommandClear();

    void PushFrontCommand(const Command& command);
    void PushFrontCommand(int target_value, float transition_duration);
    // 命令を最後尾に追加
    void PushBackCommand(const Command& command);
    void PushBackCommand(int target_value, float transition_duration);
private:
    std::deque<Command> command_pool;
    State state;
    float interpolation_timer;   // 補間に使うタイマー
    float start_value;

private:
    std::weak_ptr<TextNumberComponent> text_number_Wptr;

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

