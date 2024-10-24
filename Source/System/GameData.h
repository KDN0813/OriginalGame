#pragma once
#include "System/ClassBase/Singleton.h"
#ifdef _DEBUG
#include <vector>   // �f�o�b�O�������g�p���Ă��Ȃ�
#endif // _DEBUG


class GameData : public Singleton<GameData>
{
public:
    // �Q�[���̏��
    enum class GameStatus
    {
        DEFAULT,            // �ʏ���(�Q�[����)
        DEFEAT,             // ���@�����S�����Ƃ��i�s�k�j
        VICTORY,            // �G���S�ł����Ƃ��i�����j
        RETURN_TO_TITLE,    // �^�C�g���ɖ߂�Ƃ�
        RESTART,            // �Q�[�������X�^�[�g����Ƃ�
        PAUSED,             // �|�[�Y��

        MAX,
    };
public:
    struct GameParam
    {
        int score = 0;                                  // �X�R�A
        float game_elapsed_time = 0.0f;                 // �o�ߎ���
        GameStatus game_status = GameStatus::DEFAULT;   // �Q�[���̏��
    };
public:
    GameData();
    ~GameData() {};

    void ClearParam() { this->param = GameParam(); }

    //  ���݂̃Q�[����Ԃ��w�肳�ꂽ��Ԃ��ǂ������肷��
    bool IsCurrentGameState(GameStatus status) { return (this->param.game_status == status); };

    // �e��擾�E�ݒ�֐�
    int GetScore()const { return this->param.score; }
    float GetGameElapsedTime()const { return this->param.game_elapsed_time; }
    GameStatus GetGameStatus()const { return this->param.game_status; }
    void SetScore(int s) { this->param.score = s; }
    void SetGameElapsedTime(float t) { this->param.game_elapsed_time = t; }
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