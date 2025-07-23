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

    void AddOnScoreAdded(const std::function<void()>& function) { this->on_score_added_pool.emplace_back(function); };
    void AddOnScoreSet(const std::function<void()>& function) { this->on_score_set_pool.emplace_back(function); };

    int GetScore()const { return this->score; }
private:
    int score;

    // スコアが加算された時ののコールバック変数
    std::vector<std::function<void()>> on_score_added_pool;
    // スコアが直接設定した時のコールバック変数
    std::vector<std::function<void()>> on_score_set_pool;
};

