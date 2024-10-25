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
        DEFAULT,            // 通常状態(ゲーム中)
        DEFEAT,             // 自機が死亡したとき（敗北）
        VICTORY,            // 敵が全滅したとき（勝利）
        RETURN_TO_TITLE,    // タイトルに戻るとき
        RESTART,            // ゲームをリスタートするとき

        MAX,
    };
public:
    struct GameParam
    {
        int score = 0;                                  // スコア
        float game_elapsed_time = 0.0f;                 // 経過時間
        bool is_loading = false;                        // ロード中であるか
        bool is_pause = false;                          // ポーズ中であるか
        GameStatus game_status = GameStatus::DEFAULT;   // ゲームの状態
    };
public:
    GameData();
    ~GameData() {};

    void ClearParam() { this->param = GameParam(); }

    //  現在のゲーム状態が指定された状態かどうか判定する
    bool IsCurrentGameState(GameStatus status) { return (this->param.game_status == status); };

    // 各種取得・設定関数
    int GetScore()const { return this->param.score; }
    float GetGameElapsedTime()const { return this->param.game_elapsed_time; }
    bool GetIsLoading()const { return this->param.is_loading; }
    bool GetIsPause()const { return this->param.is_pause; }
    GameStatus GetGameStatus()const { return this->param.game_status; }
    void SetScore(int score) { this->param.score = score; }
    void SetGameElapsedTime(float game_elapsed_time) { this->param.game_elapsed_time = game_elapsed_time; }
    void SetIsLoading(bool is_loading) { this->param.is_loading = is_loading; }
    void SetIsPause(bool is_pause) { this->param.is_pause = is_pause; }
    void SetGameStatus(GameStatus status) { this->param.game_status = status; }
private:
    GameParam param{};

#ifdef _DEBUG
public:
    void DebugDrawGUI();
private:
    std::vector<std::string> game_status_name_pool;
    int select_game_status_index = 0;

#endif // _DEBUG
};