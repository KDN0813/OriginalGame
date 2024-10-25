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

        MAX,
    };
public:
    struct GameParam
    {
        int score = 0;                                  // �X�R�A
        float game_elapsed_time = 0.0f;                 // �o�ߎ���
        bool is_loading = false;                        // ���[�h���ł��邩
        bool is_pause = false;                          // �|�[�Y���ł��邩
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