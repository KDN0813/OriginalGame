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
        GAME = 0,           // �Q�[�����
        TITLE,              // �^�C�g�����
        DEFEAT,             // ���@�����S�����Ƃ��i�s�k�j
        VICTORY,            // �������Ԃ܂Ő�������(����)
        RETURN_TO_TITLE,    // �^�C�g���ɖ߂�Ƃ�
        RESTART,            // �Q�[�������X�^�[�g����Ƃ�
        
        MAX,
    };
public:
    struct GameParam
    {
        int score = 0;                                   // �X�R�A
        float game_end_timer = 100.0f;                   // �Q�[���I���܂ł̎���
        bool is_loading = false;                         // ���[�h���ł��邩
        bool is_pause = false;                           // �|�[�Y���ł��邩
        bool is_close_window = false;                    // �E�B���h�E�̏I���t���O
        GameStatus game_status = GameStatus::GAME;       // �Q�[���̏��
    };
public:
    GameData();
    ~GameData() {};

    void ClearParam() { this->param = GameParam(); }

    //  ���݂̃Q�[����Ԃ��w�肳�ꂽ��Ԃ��ǂ������肷��
    bool IsCurrentGameState(GameStatus status) { return (this->param.game_status == status); };

    // �Q�[���I���^�C�}�[�̍X�V
    void UpdateGameEndTimer(float elapsed_time) { this->param.game_end_timer -= elapsed_time; }

    // �^�C���A�b�v��Ԃł��邩
    bool IsTimeUp()const { return(this->param.game_end_timer <= 0.0f); };

    // �e��擾�E�ݒ�֐�
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
    // �E�B���h�E���I������
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
    bool draw_imgui_flag = true; // ImGui�̕\���t���O
    bool draw_debug_primitive_flag = true; // �f�o�b�O�v���~�e�B�u�̕\���t���O

#endif // _DEBUG
};