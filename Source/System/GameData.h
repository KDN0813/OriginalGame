#pragma once
#include "System/ClassBase/Singleton.h"

class GameData : public Singleton<GameData>
{
public:
    GameData() :Singleton(this) {};
    ~GameData() {};

    void ClearParam() { this->score = 0; this->game_elapsed_time = 0.0f; }

    // �e��擾�E�ݒ�֐�
    int GetScore()const { return this->score; }
    float GetGameElapsedTime()const { return this->game_elapsed_time; }
    void SetScore(int s) { this->score = s; }
    void SetGameElapsedTime(float t) { this->game_elapsed_time = t; }
private:
    int score = 0;              // �X�R�A
    float game_elapsed_time = 0.0f;  // �o�ߎ���
};