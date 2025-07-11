#include "ChainScoreCounterComponent.h"
#include <string>

#ifdef _DEBUG
#include <Imgui.h>
#endif // _DEBUG

void ChainScoreCounterComponent::ReStart()
{
    this->param.chain_score = 0;
    this->param.chain_timer = 0.0f;
    this->param.is_reset = false;
}

void ChainScoreCounterComponent::Update(float elapsed_time)
{
    if (this->param.chain_timer <= 0.0f)
    {
        if (!this->param.is_reset)
        {
            ChainEnd();
        }
    }
    else
    {
        this->param.is_reset = false;
        this->param.chain_timer -= elapsed_time;   // タイマー更新
    }
}

void ChainScoreCounterComponent::ChainEnd()
{
    this->param.chain_score = 0;
    this->param.is_reset = true;
    if (OnScoreChainEnd)
    {
        OnScoreChainEnd();
    }
}

void ChainScoreCounterComponent::AddChain(int add_score)
{
    // 連鎖中でないなら
    if (this->param.chain_timer <= 0.0f)
    {
        if (OnScoreChainStart)
        {
            OnScoreChainStart();
        }
    }

    this->param.chain_timer = this->param.chain_timer_max;    // タイマーの設定
    this->param.chain_score += add_score;

    if (OnScoreAdded)
    {
        OnScoreAdded(this->param.chain_score);
    }
}

#ifdef _DEBUG

void ChainScoreCounterComponent::DrawDebugGUI()
{
    ImGui::SliderFloat("Chain Timer Max", &this->param.chain_timer, 0.0f, this->param.chain_timer_max);
    ImGui::InputFloat("Chain Timer Max", &this->param.chain_timer_max);
    ImGui::Text(std::string("Chain Score: " + std::to_string(this->param.chain_score)).c_str());

    if (ImGui::Button("AddChain")) AddChain(1);
    if (ImGui::Button("ChainEnd")) ChainEnd();
}

#endif // _DEBUG