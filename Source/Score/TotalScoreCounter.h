#pragma once
#include <functional>
#include <vector>

// �����X�R�A���Ǘ�����
class TotalScoreCounter
{
public:
    TotalScoreCounter() : score(0){};
    ~TotalScoreCounter() {};

    void AddScore(int value);
    void SetScore(int value);
    void Reset();

    void AddOnScoreAdded(const std::function<void()>& function) { this->on_score_added_pool.emplace_back(function); };
    void AddOnScoreSet(const std::function<void()>& function) { this->on_score_set_pool.emplace_back(function); };

    int GetScore()const { return this->score; }
private:
    int score;

    // �X�R�A�����Z���ꂽ���̂̃R�[���o�b�N�ϐ�
    std::vector<std::function<void()>> on_score_added_pool;
    // �X�R�A�����ڐݒ肵�����̃R�[���o�b�N�ϐ�
    std::vector<std::function<void()>> on_score_set_pool;
};

