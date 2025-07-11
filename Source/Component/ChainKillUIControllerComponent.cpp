#include "ChainKillUIControllerComponent.h"

void ChainKillUIControllerComponent::ReStart()
{
    this->param.draw_chain_kill_count = 0.0f;
}

void ChainKillUIControllerComponent::Update(float elapsed_time)
{
}

#ifdef _DEBUG

void ChainKillUIControllerComponent::DrawDebugGUI()
{
}

#endif // _DEBUG