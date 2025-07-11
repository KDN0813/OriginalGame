#include "ChainScoreUIControllerComponent.h"

#include "Object/Object.h"
#include "TextNumberComponent.h"

void ChainScoreUIControllerComponent::ReStart()
{
}

void ChainScoreUIControllerComponent::Update(float elapsed_time)
{
}

void ChainScoreUIControllerComponent::OnScoreAdded(int value)
{
    const auto& owner = GetOwner();
    if (!owner) return;
    const auto& text_numbe = owner->GetComponent(this->text_number_Wptr);
    if (!text_numbe) return;
    text_numbe->SetDrawValue(value);
}

void ChainScoreUIControllerComponent::OnScoreChainEnded()
{
    const auto& owner = GetOwner();
    if (!owner) return;
    const auto& text_numbe = owner->GetComponent(this->text_number_Wptr);
    if (!text_numbe) return;
    text_numbe->SetDrawValue(0);
}

#ifdef _DEBUG

void ChainScoreUIControllerComponent::DrawDebugGUI()
{
}

#endif // _DEBUG