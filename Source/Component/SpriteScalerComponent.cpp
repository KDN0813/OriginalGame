#include "SpriteScalerComponent.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

#include <cmath>
#include "Object/Object.h"
#include "Component/BaseSpriteComponent.h"
#include "Component/Transform2DComponent.h"

void SpriteScalerComponent::ReStart()
{
    this->interpolation_timer = 0.0f;
    this->state = State::Start;
    while (!this->command_pool.empty()) 
    {
        this->command_pool.pop();
    }
}

void SpriteScalerComponent::Update(float elapsed_time)
{
    if (this->command_pool.empty()) return;

    ScaleCommand& current_command = this->command_pool.front();

    // 必用コンポーネント取得
    const auto& owner = GetOwner();
    if (!owner) return;
    const auto& sprite = owner->GetComponent(this->sprite_Wptr);
    if (!sprite) return;
    const auto& transform = owner->GetComponent(this->transform_Wptr);
    if (!transform) return;

    // 初期設定
    switch (this->state)
    {
    case State::Start:
    {
        // 初期化
        this->start_scale = transform->GetLocalScale();
        this->interpolation_timer = 0.0f;
        this->state = State::Run;
        break;
    }
    case State::Run:
    {
        // 更新
        DirectX::XMFLOAT2 scale{};
        const float t = this->interpolation_timer / current_command.transition_duration;
        scale.x = std::lerp(this->start_scale.x, current_command.target_scale.x, t);
        scale.y = std::lerp(this->start_scale.y, current_command.target_scale.y, t);

        transform->SetLocalScale(scale);

        // 修了判定
        if (current_command.transition_duration <= this->interpolation_timer)
        {
            this->state = State::End;
        }
        break;
    }
    case State::End:
    {
        // 修了
        this->command_pool.pop();   // 要素を削除
        this->state = State::Start;
        break;
    }
    default:
        break;
    }

    this->interpolation_timer += elapsed_time;
}

void SpriteScalerComponent::PushCommand(const ScaleCommand& command)
{
    this->command_pool.push(command);
}

void SpriteScalerComponent::PushCommand(DirectX::XMFLOAT2 target_scale, float transition_duration)
{
    PushCommand(ScaleCommand(target_scale, transition_duration));
}

void SpriteScalerComponent::PushCommand(float target_scale, float transition_duration)
{
    PushCommand(ScaleCommand({ target_scale ,target_scale }, transition_duration));
}

#ifdef _DEBUG

void SpriteScalerComponent::DrawDebugGUI()
{
    ImGui::DragFloat("Debug Change Scale", &this->debug_change_scale);
    ImGui::DragFloat("Debug Transition Duration", &this->debug_transition_duration);

    if (ImGui::Button("PushCommand"))
    {
        PushCommand(this->debug_change_scale, this->debug_transition_duration);
    }
}

#endif // _DEBUG