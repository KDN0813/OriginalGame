#include "ScoreUIValueAnimatorComponent.h"

#ifdef _DEBUG
#include<imgui.h>
#endif // _DEBUG

#include "System/GameData.h"
#include "Object/Object.h"
#include "TextNumberValueInterpolatorComponent.h"
#include "SpriteScalerComponent.h"

void ScoreUIValueAnimatorComponent::OnScoreAdded()
{
    const auto& owner = GetOwner();
    if (!owner) return;

    GameData::Instance game_data = GameData::GetInstance();

    const auto& value_interpolator = owner->GetComponent(this->value_interpolator_Wptr);
    if (!value_interpolator) return;

    value_interpolator->CommandClear();
    value_interpolator->PushBackCommand(game_data->GetScore(), this->param.animetion_total_time);

    const auto& sprite_scaler = owner->GetComponent(this->sprite_scaler_Wptr);
    if (!sprite_scaler) return;

    sprite_scaler->CommandClear();
    sprite_scaler->PushBackCommand(this->param.expanded.target_scale, this->param.expanded.time);// �X�v���C�g�̊g��w��
    sprite_scaler->PushBackCommand(this->param.shrink.target_scale, this->param.shrink.time);// �X�v���C�g�̏k���w��
}

void ScoreUIValueAnimatorComponent::OnScoreSet()
{
    const auto& owner = GetOwner();
    if (!owner) return;

    GameData::Instance game_data = GameData::GetInstance();

    const auto& value_interpolator = owner->GetComponent(this->value_interpolator_Wptr);
    if (!value_interpolator) return;

    // ���o�Ȃ��Œl��ύX����
    value_interpolator->PushBackCommand(game_data->GetScore(), 0.0f);
}

#ifdef _DEBUG

void ScoreUIValueAnimatorComponent::DrawDebugGUI()
{
    ImGui::InputFloat("Animation Time",&this->param.animetion_total_time);

    ImGui::InputFloat2("Expanded Scale", &this->param.expanded.target_scale.x);
    ImGui::InputFloat("Expanded Time", &this->param.expanded.time);

    ImGui::InputFloat2("Shrink Scale", &this->param.shrink.target_scale.x);
    ImGui::InputFloat("Shrink Time", &this->param.shrink.time);

    if (ImGui::Button("ScoreChainEnd"))
    {
        OnScoreAdded();
    }
}

#endif // _DEBUG