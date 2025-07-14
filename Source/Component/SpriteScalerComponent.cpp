#include "SpriteScalerComponent.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

#include <cmath>
#include <algorithm>
#include "Object/Object.h"
#include "Component/BaseSpriteComponent.h"
#include "Component/Transform2DComponent.h"

void SpriteScalerComponent::ReStart()
{
    this->interpolation_timer = 0.0f;
    CommandClear();
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
        // 補間時間が0以下なら即移動して終了
        if (current_command.transition_duration <= 0.0f)
        {
            transform->SetLocalPosition(current_command.target_scale);
            this->state = State::End;
            break;
        }

        // t:補間係数
        const float t = std::clamp(
            this->interpolation_timer / current_command.transition_duration, 0.0f, 1.0f);

        // 線形補間
        const DirectX::XMFLOAT2 interpolated_scale
        {
            std::lerp(this->start_scale.x, current_command.target_scale.x, t),
            std::lerp(this->start_scale.y, current_command.target_scale.y, t)
        };

        transform->SetLocalScale(interpolated_scale);

        // 補間時間を超えたら終了
        if (current_command.transition_duration <= this->interpolation_timer)
        {
            this->state = State::End;
        }
        break;
    }
    case State::End:
    {
        // 終了
        this->command_pool.pop_front();   // 要素を削除
        this->state = State::Start;
        break;
    }
    default:
        break;
    }

    this->interpolation_timer += elapsed_time;
}

void SpriteScalerComponent::PushFrontCommand(const ScaleCommand& command)
{
    this->state = State::Start;
    this->command_pool.push_front(command);
}

void SpriteScalerComponent::PushFrontCommand(DirectX::XMFLOAT2 target_scale, float transition_duration)
{
    PushFrontCommand(ScaleCommand(target_scale, transition_duration));
}

void SpriteScalerComponent::PushFrontCommand(float target_scale, float transition_duration)
{
    PushFrontCommand(ScaleCommand({ target_scale ,target_scale }, transition_duration));
}

void SpriteScalerComponent::PushBackCommand(const ScaleCommand& command)
{
    this->command_pool.push_back(command);
}

void SpriteScalerComponent::PushBackCommand(DirectX::XMFLOAT2 target_scale, float transition_duration)
{
    PushBackCommand(ScaleCommand(target_scale, transition_duration));
}

void SpriteScalerComponent::PushBackCommand(float target_scale, float transition_duration)
{
    PushBackCommand(ScaleCommand({ target_scale ,target_scale }, transition_duration));
}

void SpriteScalerComponent::CommandClear()
{
    while (!this->command_pool.empty())
    {
        this->command_pool.pop_front();
    }
    this->state = State::Start;
}

#ifdef _DEBUG

void SpriteScalerComponent::DrawDebugGUI()
{
    ImGui::DragFloat("Debug Change Pos", &this->debug_change_scale);
    ImGui::DragFloat("Debug Transition Duration##SpriteScalerComponent", &this->debug_transition_duration);

    if (ImGui::Button("PushFrontCommand##SpriteScalerComponent"))
    {
        PushFrontCommand(this->debug_change_scale, this->debug_transition_duration);
    }
    if (ImGui::Button("PushBackCommand##SpriteScalerComponent"))
    {
        PushBackCommand(this->debug_change_scale, this->debug_transition_duration);
    }
    if (ImGui::Button("CommandClear##SpriteScalerComponent"))
    {
        CommandClear();
    }

    if (ImGui::TreeNodeEx("Commands##SpriteScalerComponent"))
    {
        int count = 0;
        for (auto command : this->command_pool)
        {
            ImGui::InputFloat2(std::string("Target Scale##SpriteScalerComponent" + std::to_string(count)).c_str(), &command.target_scale.x);
            ImGui::InputFloat2(std::string("Transition Duration##SpriteScalerComponent" + std::to_string(count)).c_str(), &command.transition_duration);

            ImGui::Spacing();
        }
        ImGui::TreePop();
    }
}

#endif // _DEBUG