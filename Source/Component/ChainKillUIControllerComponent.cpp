#include "ChainKillUIControllerComponent.h"

#include "Object/Object.h"
#include "TextNumberComponent.h"

void ChainKillUIControllerComponent::ReStart()
{
}

void ChainKillUIControllerComponent::Update(float elapsed_time)
{
}

void ChainKillUIControllerComponent::OnKillCountAdded(int value)
{
    const auto& owner = GetOwner();
    if (!owner) return;
    const auto& text_numbe = owner->GetComponent(this->text_numbe_Wptr);
    if (!text_numbe) return;
    text_numbe->SetDrawValue(value);
}

void ChainKillUIControllerComponent::OnChainKillEnded()
{
    const auto& owner = GetOwner();
    if (!owner) return;
    const auto& text_numbe = owner->GetComponent(this->text_numbe_Wptr);
    if (!text_numbe) return;
    text_numbe->SetDrawValue(0);
}

#ifdef _DEBUG

void ChainKillUIControllerComponent::DrawDebugGUI()
{
}

#endif // _DEBUG