#include "TextNumberValueInterpolatorComponent.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

#include <cmath>
#include <algorithm>
#include "Object/Object.h"
#include "TextNumberComponent.h"

void TextNumberValueInterpolatorComponent::ReStart()
{
    this->interpolation_timer = 0.0f;
    CommandClear();
}

void TextNumberValueInterpolatorComponent::Update(float elapsed_time)
{
    if (this->command_pool.empty()) return;

    Command& current_command = this->command_pool.front();

    // 必用コンポーネント取得
    const auto& owner = GetOwner();
    if (!owner) return;
    const auto& text_number = owner->GetComponent(this->text_number_Wptr);

    // 初期設定
    switch (this->state)
    {
    case State::Start:
    {
        // 初期化
        this->start_value = text_number->GetDrawValue();
        this->interpolation_timer = 0.0f;
        this->state = State::Run;
        break;
    }
    case State::Run:
    {
        // 補間時間が0以下なら即移動して終了
        if (current_command.transition_duration <= 0.0f)
        {
            text_number->SetDrawValue(current_command.target_value);
            this->state = State::End;
            break;
        }

        // t:補間係数
        const float t = std::clamp(
            this->interpolation_timer / current_command.transition_duration, 0.0f, 1.0f);

        // 線形補間
        const int interpolated_value = static_cast<int>(std::lerp(static_cast<float>(this->start_value), static_cast<float>(current_command.target_value), t));

        text_number->SetDrawValue(interpolated_value);

        // 補間時間を超えたら終了
        if (this->interpolation_timer >= current_command.transition_duration)
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

void TextNumberValueInterpolatorComponent::CommandClear()
{
    while (!this->command_pool.empty())
    {
        this->command_pool.pop_front();
    }
    this->state = State::Start;
}

void TextNumberValueInterpolatorComponent::PushFrontCommand(const Command& command)
{
    this->state = State::Start;
    this->command_pool.push_front(command);
}

void TextNumberValueInterpolatorComponent::PushFrontCommand(int target_value, float transition_duration)
{
    PushFrontCommand({ target_value ,transition_duration });
}

void TextNumberValueInterpolatorComponent::PushBackCommand(const Command& command)
{
    this->command_pool.push_back(command);
}

void TextNumberValueInterpolatorComponent::PushBackCommand(int target_value, float transition_duration)
{
    PushBackCommand({ target_value ,transition_duration });
}

#ifdef _DEBUG

void TextNumberValueInterpolatorComponent::DrawDebugGUI()
{
    if (ImGui::Button("CommandClear"))
    {
        CommandClear();
    }

    if (ImGui::TreeNodeEx("Commands"))
    {
        int count = 0;
        for (auto command : this->command_pool)
        {
            ImGui::InputInt(std::string("Target Value##ValueInterpolator" + std::to_string(count)).c_str(), &command.target_value);
            ImGui::InputFloat(std::string("Transition Duration##ValueInterpolator" + std::to_string(count)).c_str(), &command.transition_duration);

            ImGui::Spacing();
        }
        ImGui::TreePop();
    }
}

#endif // _DEBUG