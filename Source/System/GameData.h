#pragma once
#include "System/ClassBase/Singleton.h"
#ifdef _DEBUG
#include <vector>   // デバッグ時しか使用していない
#endif // _DEBUG


class GameData : public Singleton<GameData>
{
public:
    // ゲームの状態
    enum class GameStatus
    {
        GAME = 0,           // ゲーム状態
        TITLE,              // タイトル状態
        DEFEAT,             // 自機が死亡したとき（敗北）
        VICTORY,            // 制限時間まで生存した(勝利)
        RETURN_TO_TITLE,    // タイトルに戻るとき
        RESTART,            // ゲームをリスタートするとき
        
        MAX,
    };
public:
    struct GameParam
    {
        int score = 0;                                   // スコア
        float game_end_timer = 100.0f;                   // ゲーム終了までの時間
        bool is_loading = false;                         // ロード中であるか
        bool is_pause = false;                           // ポーズ中であるか
        bool is_close_window = false;                    // ウィンドウの終了フラグ
        GameStatus game_status = GameStatus::GAME;       // ゲームの状態
    };
public:
    GameData();
    ~GameData() {};

    void ClearParam() { this->param = GameParam(); }

    //  現在のゲーム状態が指定された状態かどうか判定する
    bool IsCurrentGameState(GameStatus status) { return (this->param.game_status == status); };

    // ゲーム終了タイマーの更新
    void UpdateGameEndTimer(float elapsed_time) { this->param.game_end_timer -= elapsed_time; }

    // タイムアップ状態であるか
    bool IsTimeUp()const { return(this->param.game_end_timer <= 0.0f); };

    // 各種取得・設定関数
    int GetScore()const { return this->param.score; }
    float GetGameEndTimer()const { return this->param.game_end_timer; }
    bool GetIsLoading()const { return this->param.is_loading; }
    bool GetIsPause()const { return this->param.is_pause; }
    GameStatus GetGameStatus()const { return this->param.game_status; }
    void SetScore(int score) { this->param.score = score; }
    void AddScore(int score) { this->param.score += score; }
    void RestGameTime() { this->param.game_end_timer = GameParam().game_end_timer; }
    void SetGameElapsedTime(float game_elapsed_time) { this->param.game_end_timer = game_elapsed_time; }
    void SetIsLoading(bool is_loading) { this->param.is_loading = is_loading; }
    void SetIsPause(bool is_pause) { this->param.is_pause = is_pause; }
    void SetGameStatus(GameStatus status) { this->param.game_status = status; }
    // ウィンドウを終了する
    void CloseWindow() { this->param.is_close_window = true; }
    bool GetIsCloseWindow()const { return this->param.is_close_window; }
private:
    GameParam param{};

#ifdef _DEBUG
public:
    void DebugDrawGUI();
    bool GetDrawImguiFlag() const { return this->draw_imgui_flag; }
    void SetDrawImguiFlag(bool flag) { this->draw_imgui_flag = flag; }
    bool GetDrawDebugPrimitiveFlag() const { return this->draw_debug_primitive_flag; }
    void SetDrawDebugPrimitiveFlag(bool flag) { this->draw_debug_primitive_flag = flag; }
private:
    std::vector<std::string> game_status_name_pool;
    int select_game_status_index = 0;
    bool draw_imgui_flag = true; // ImGuiの表示フラグ
    bool draw_debug_primitive_flag = true; // デバッグプリミティブの表示フラグ

#endif // _DEBUG
};