#pragma once
#include "System/ClassBase/Singleton.h"

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
        PAUSED              // ポーズ中
    };
public:
    struct GameParam
    {
        int score = 0;                                  // スコア
        float game_elapsed_time = 0.0f;                 // 経過時間
        GameStatus game_status = GameStatus::DEFAULT;   // ゲームの状態
    };
public:
    GameData() :Singleton(this) {};
    ~GameData() {};

    void ClearParam() { this->param.score = 0; this->param.game_elapsed_time = 0.0f; }

    //  現在のゲーム状態が指定された状態かどうか判定する
    bool IsCurrentGameState(GameStatus status) { return (this->param.game_status == status); };

    // 各種取得・設定関数
    int GetScore()const { return this->param.score; }
    float GetGameElapsedTime()const { return this->param.game_elapsed_time; }
    GameStatus GetGameStatus()const { return this->param.game_status; }
    void SetScore(int s) { this->param.score = s; }
    void SetGameElapsedTime(float t) { this->param.game_elapsed_time = t; }
    void SetGameStatus(GameStatus status) { this->param.game_status = status; }
private:
    GameParam param{};
};