#include "ScoreUIValueAnimatorComponent.h"

#ifdef _DEBUG
#include<imgui.h>
#endif // _DEBUG

#include "System/GameData.h"
#include "Object/Object.h"
#include "TextNumberValueInterpolatorComponent.h"

void ScoreUIValueAnimatorComponent::ScoreChainEnd()
{
    const auto& owner = GetOwner();
    if (!owner) return;

    GameData::Instance game_data = GameData::GetInstance();

    const auto& value_interpolator = owner->GetComponent(this->value_interpolator_Wptr);
    if (!value_interpolator) return;

    value_interpolator->PushBackCommand(game_data->GetScore(), this->param.animetion_time);
}

#ifdef _DEBUG

void ScoreUIValueAnimatorComponent::DrawDebugGUI()
{
    ImGui::InputFloat("Animation Time",&this->param.animetion_time);

    if (ImGui::Button("ScoreChainEnd"))
    {
        ScoreChainEnd();
    }
}

#endif // _DEBUG