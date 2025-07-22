#include "SpriteShakeComponent.h"

#ifdef _DEBUG
#include "Debug/ImguiHelper.h"
#endif // _DEBUG

#include <cmath>
#include <algorithm>
#include "Object/Object.h"
#include "System/MyMath/MyMathf.h"
#include "Component/BaseSpriteComponent.h"

void SpriteShakeComponent::ReStart()
{
    this->tiemr = 0.0f;
    this->shake_movement = {};
    while (!this->command_pool.empty())
    {
        this->command_pool.pop_front();
    }
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

    if (current_command.end_time > 0.0f)
    {
        // シェイク量をランダム計算
        const float TIME_RATE = this->tiemr / current_command.end_time;
        const DirectX::XMFLOAT2 movement = 
        {
            current_command.force.x - (current_command.force.x * TIME_RATE),
            current_command.force.x - (current_command.force.y * TIME_RATE)
        } ;
        this->shake_movement.x = MyMath::RandomRange(-movement.x, +movement.x);
        this->shake_movement.y = MyMath::RandomRange(-movement.y, +movement.y);

        sprite->SetOffset(this->shake_movement);

        //　タイマー更新
        this->tiemr = (std::min)(current_command.end_time, this->tiemr + elapsed_time);
        if (this->tiemr >= current_command.end_time) 
        { 
            this->command_pool.pop_front();     // 先頭の要素を削除
            this->tiemr = 0.0f;
            sprite->SetOffset({ 0.0f ,0.0f });  // offset初期化
        }
    }
}

void SpriteShakeComponent::PushFrontCommand(const ShakeCommand& command)
{
    this->tiemr = 0.0f;
    this->command_pool.push_front(command);
}

void SpriteShakeComponent::PushFrontCommand(DirectX::XMFLOAT2 force, float shake_time)
{
    PushFrontCommand(ShakeCommand(force, shake_time));
}

void SpriteShakeComponent::PushBackCommand(const ShakeCommand& command)
{
    this->command_pool.push_back(command);
}

void SpriteShakeComponent::PushBackCommand(DirectX::XMFLOAT2 force, float shake_time)
{
    PushFrontCommand(ShakeCommand(force, shake_time));
}

#ifdef _DEBUG

void SpriteShakeComponent::DrawDebugGUI()
{
    ImGui::DragFloat2("Debug Force", &this->debug_force.x);
    ImGui::DragFloat2("Debug End Time", &this->debug_end_time);

    if (ImGui::Button("PushFrontCommand##SpriteShakeComponent"))
    {
        PushFrontCommand(this->debug_force, this->debug_end_time);
    }
    if (ImGui::Button("PushBackCommand##SpriteShakeComponent"))
    {
        PushBackCommand(this->debug_force, this->debug_end_time);
    }
    if (ImGui::Button("CommandClear##SpriteShakeComponent"))
    {
        ReStart();
    }
}

#endif // _DEBUG