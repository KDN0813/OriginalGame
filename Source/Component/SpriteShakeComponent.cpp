#include "SpriteShakeComponent.h"

#ifdef _DEBUG
#include "Debug/ImguiHelper.h"
#endif // _DEBUG

#include <cmath>
#include <algorithm>
#include "Object/Object.h"
#include "Component/BaseSpriteComponent.h"
#include "Component/Transform2DComponent.h"

void SpriteShakeComponent::ReStart()
{
    this->interpolation_timer = 0.0f;
    CommandClear();
}

void SpriteShakeComponent::Update(float elapsed_time)
{
    if (this->command_pool.empty()) return;

    ShakeCommand& current_command = this->command_pool.front();

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

void SpriteShakeComponent::PushFrontCommand(const ShakeCommand& command)
{
    this->state = State::Start;
    this->command_pool.push_front(command);
}

void SpriteShakeComponent::PushFrontCommand(DirectX::XMFLOAT2 target_scale, float transition_duration)
{
    PushFrontCommand(ShakeCommand(target_scale, transition_duration));
}

void SpriteShakeComponent::PushBackCommand(const ShakeCommand& command)
{
    this->command_pool.push_back(command);
}

void SpriteShakeComponent::PushBackCommand(DirectX::XMFLOAT2 target_scale, float transition_duration)
{
    PushBackCommand(ShakeCommand(target_scale, transition_duration));
}

void SpriteShakeComponent::CommandClear()
{
    while (!this->command_pool.empty())
    {
        this->command_pool.pop_front();
    }
    this->state = State::Start;
}

#ifdef _DEBUG

void SpriteShakeComponent::DrawDebugGUI()
{
}

#endif // _DEBUG