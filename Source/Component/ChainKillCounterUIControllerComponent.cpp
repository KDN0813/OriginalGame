#include "ChainKillCounterUIControllerComponent.h"

void ChainKillCounterUIControllerComponent::ReStart()
{
    this->param.chain_kill_timer = 0.0f;
}

void ChainKillCounterUIControllerComponent::Update(float elapsed_time)
{
}

#ifdef _DEBUG

void ChainKillCounterUIControllerComponent::DrawDebugGUI()
{
}

#endif // _DEBUG