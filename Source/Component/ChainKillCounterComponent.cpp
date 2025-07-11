#include "ChainKillCounterComponent.h"
#include <string>

#ifdef _DEBUG
#include <Imgui.h>
#endif // _DEBUG


void ChainKillCounterComponent::ReStart()
{
    this->param.chain_kill_timer = 0.0f;
}

void ChainKillCounterComponent::Update(float elapsed_time)
{
    if (this->param.chain_kill_timer <= 0.0f)
    {
        this->param.chain_kill_count = 0;
    }
    this->param.chain_kill_timer -= elapsed_time;   // �^�C�}�[�X�V
}


void ChainKillCounterComponent::AddChainKill()
{
    this->param.chain_kill_timer = this->param.chain_kill_timer_max;    // �^�C�}�[�̐ݒ�
    ++this->param.chain_kill_count;
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