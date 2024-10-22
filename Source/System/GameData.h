#pragma once
#include "System/ClassBase/Singleton.h"

class GameData : public Singleton<GameData>
{
public:
    GameData() :Singleton(this) {};
    ~GameData() {};

    void ClearParam() { this->score = 0; this->game_elapsed_time = 0.0f; }

    // 各種取得・設定関数
    int GetScore()const { return this->score; }
    float GetGameElapsedTime()const { return this->game_elapsed_time; }
    void SetScore(int s) { this->score = s; }
    void SetGameElapsedTime(float t) { this->game_elapsed_time = t; }
private:
    int score = 0;              // スコア
    float game_elapsed_time = 0.0f;  // 経過時間
};