#include "ChainKillCounterComponent.h"
#include <string>

#ifdef _DEBUG
#include <Imgui.h>
#endif // _DEBUG

void ChainKillCounterComponent::ReStart()
{
    this->param.chain_kill_count = 0;
    this->param.chain_kill_timer = 0.0f;
    this->param.is_reset = false;
}

void ChainKillCounterComponent::Update(float elapsed_time)
{
    if (this->param.chain_kill_timer <= 0.0f)
    {
        if (this->param.is_reset)
        {
            this->param.chain_kill_count = 0;
            this->param.is_reset = true;
            if (onKillCountReset)
            {
                onKillCountReset();
            }
        }
    }
    else
    {
        this->param.is_reset = false;
    }
    this->param.chain_kill_timer -= elapsed_time;   // タイマー更新
}

void ChainKillCounterComponent::SetOnKillCountAdded(const std::function<void(int)>&)
{
}

void ChainKillCounterComponent::SetOnKillCountReset(const std::function<void()>&)
{
}

void ChainKillCounterComponent::AddChainKill()
{
    this->param.chain_kill_timer = this->param.chain_kill_timer_max;    // タイマーの設定
    ++this->param.chain_kill_count;

    if (onKillCountAdded)
    {
        onKillCountAdded(this->param.chain_kill_count);
    }
}

#ifdef _DEBUG

void ChainKillCounterComponent::DrawDebugGUI()
{
    ImGui::SliderFloat("Chain Timer Max", &this->param.chain_kill_timer, 0.0f, this->param.chain_kill_timer_max);
    ImGui::InputFloat("Chain Timer Max", &this->param.chain_kill_timer_max);
    ImGui::Text(std::string("Chain Count" + std::to_string(this->param.chain_kill_count)).c_str());

    if (ImGui::Button("AddChain")) AddChainKill();
}

#endif // _DEBUG