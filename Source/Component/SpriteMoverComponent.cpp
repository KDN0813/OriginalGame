#include "SpriteMoverComponent.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

#include <cmath>
#include "Object/Object.h"
#include "Component/BaseSpriteComponent.h"
#include "Component/Transform2DComponent.h"

void SpriteMoverComponent::ReStart()
{
    this->interpolation_timer = 0.0f;
    CommandClear();
}

void SpriteMoverComponent::Update(float elapsed_time)
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
        this->start_pos = transform->GetLocalPosition();
        this->interpolation_timer = 0.0f;
        this->state = State::Run;
        break;
    }
    case State::Run:
    {
        // 更新
        DirectX::XMFLOAT2 pos{};
        const float t = this->interpolation_timer / current_command.transition_duration;
        pos.x = std::lerp(this->start_pos.x, current_command.target_pos.x, t);
        pos.y = std::lerp(this->start_pos.y, current_command.target_pos.y, t);

        transform->SetLocalPosition(pos);

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
        this->command_pool.pop_front();   // 要素を削除
        this->state = State::Start;
        break;
    }
    default:
        break;
    }

    this->interpolation_timer += elapsed_time;
}

void SpriteMoverComponent::PushFrontCommand(const ScaleCommand& command)
{
    this->state = State::Start;
    this->command_pool.push_front(command);
}

void SpriteMoverComponent::PushFrontCommand(DirectX::XMFLOAT2 target_scale, float transition_duration)
{
    PushFrontCommand(ScaleCommand(target_scale, transition_duration));
}

void SpriteMoverComponent::PushBackCommand(const ScaleCommand& command)
{
    this->command_pool.push_back(command);
}

void SpriteMoverComponent::PushBackCommand(DirectX::XMFLOAT2 target_scale, float transition_duration)
{
    PushBackCommand(ScaleCommand(target_scale, transition_duration));
}


void SpriteMoverComponent::CommandClear()
{
    while (!this->command_pool.empty())
    {
        this->command_pool.pop_front();
    }
    this->state = State::Start;
}

#ifdef _DEBUG

void SpriteMoverComponent::DrawDebugGUI()
{
    ImGui::DragFloat2("Debug Change Scale", &this->debug_change_pos.x);
    ImGui::DragFloat("Debug Transition Duration", &this->debug_transition_duration);

    if (ImGui::Button("PushFrontCommand"))
    {
        PushFrontCommand(this->debug_change_pos, this->debug_transition_duration);
    }
    if (ImGui::Button("PushBackCommand"))
    {
        PushBackCommand(this->debug_change_pos, this->debug_transition_duration);
    }
    if (ImGui::Button("CommandClear"))
    {
        CommandClear();
    }

    if (ImGui::TreeNodeEx("Commands"))
    {
        int count = 0;
        for (auto command : this->command_pool)
        {
            ImGui::InputFloat2(std::string("Target Pos##" + std::to_string(count)).c_str(), &command.target_pos.x);
            ImGui::InputFloat2(std::string("Transition Duration##" + std::to_string(count)).c_str(), &command.transition_duration);

            ImGui::Spacing();
        }
        ImGui::TreePop();
    }
}

#endif // _DEBUG