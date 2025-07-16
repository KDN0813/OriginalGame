#pragma once
#include <functional>
#include <vector>

// 総合スコアを管理する
class TotalScoreCounter
{
public:
    TotalScoreCounter() : score(0){};
    ~TotalScoreCounter() {};

    void AddScore(int value);
    void SetScore(int value);
    void Reset();

    void AddOnChangeScore(const std::function<void()>& function) { this->on_change_score_pool.emplace_back(function); };

    int GetScore()const { return this->score; }
private:
    int score;

    std::vector<std::function<void()>> on_change_score_pool;        // スコアが変化時のコールバック変数
};

