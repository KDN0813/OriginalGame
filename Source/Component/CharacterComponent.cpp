#include "CharacterComponent.h"
#ifdef _DEBUG
#include <imgui.h>
#include <string>
#include "Debug\DebugManager.h"
#include "Object\Object.h"
#include "TransformComponent.h"
#endif // _DEBUG


CharacterComponent::CharacterComponent(CharacterParam param)
    :param(param), default_param(param)
{
#ifdef _DEBUG
    this->character_radius.SetColor({ 0.0f ,1.0f ,0.0f ,1.0f });
    this->character_radius.SetRadius(this->param.radius);
    this->character_radius.SetHeight(3.0f);
#endif // _DEBUG
}

void CharacterComponent::ReStart()
{
    this->param = this->default_param;
}

void CharacterComponent::Update(float elapsed_time)
{
    this->param.invincible_timer = (std::max)(this->param.invincible_timer - elapsed_time, 0.0f);   // 無敵時間更新
    this->param.old_hp = this->param.hp;

#ifdef _DEBUG
    if (const auto& owner = GetOwner())
    {
        if (const auto& transform = owner->GetComponent<Transform3DComponent>(this->transform_Wptr))
        {
            this->character_radius.SetRadius(this->param.radius);
            this->character_radius.SetPosition(transform->GetWorldPosition());
        }
    }
#endif // _DEBUG
}

void CharacterComponent::ApplyDamage(int damage, float invincible_timer)
{
    if (damage <= 0) return;
    if (this->param.invincible_flag) return;            // 無敵中ならDamageを受けない
    if (0.0f < this->param.invincible_timer) return;    // 無敵中ならDamageを受けない
    this->param.hp = (std::max)(this->param.hp - damage, 0);

    this->param.invincible_timer = invincible_timer;
}

bool CharacterComponent::IsAlive()
{
    return (0 < this->param.hp);    // 体力が0以上なら生存
}

bool CharacterComponent::IsDamage()
{
    return (this->param.hp < this->param.old_hp);
}

bool CharacterComponent::IsInvincible()
{
    if (this->param.invincible_flag) return true;            // 無敵中ならDamageを受けない
    if (0.0f < this->param.invincible_timer) return true;    // 無敵中ならDamageを受けない

    return false;
}

#ifdef _DEBUG

void CharacterComponent::DrawDebugGUI()
{
    std::string is_invincible_text = "Invincibility:";
    is_invincible_text +=
        (this->param.invincible_flag || 0.0f < this->param.invincible_timer) ?
        "Active" : "Not Active";
    ImGui::Text(is_invincible_text.c_str());
    ImGui::Checkbox("Invincible Flag", &this->param.invincible_flag);
    if (ImGui::InputInt("HP", &this->param.hp)) this->param.hp = (std::min)((std::max)(this->param.hp, 0), this->param.max_hp);
    ImGui::InputInt("Max HP", &this->param.max_hp);
    ImGui::InputFloat("Invincible Timer", &this->param.invincible_timer);
    ImGui::DragFloat("Radius", &this->param.radius, 0.1f, 0.01f, 10.0f);

    // デバッグ用ダメージボタン
    ImGui::InputInt("Test Damage", &this->test_damage);
    if (ImGui::Button("Test ApplyDamage"))
    {
        ApplyDamage(this->test_damage, 0.0f);
    }

    // デバッグ用死亡ボタン
    ImGui::InputInt("Test Damage", &this->test_damage);
    if (ImGui::Button("Test Death"))
    {
        this->param.hp = 0;
    }

    bool is_alive = IsAlive();
    ImGui::Checkbox("Is Alive", &is_alive);
}

void CharacterComponent::DrawDebugPrimitive()
{
    DebugManager::Instance debug_manager = DebugManager::GetInstance();
    const auto& debug_primitive_renderer = debug_manager->GetDebugPrimitiveRenderer();
    if (!debug_primitive_renderer) return;
    debug_primitive_renderer->DrawCylinder(this->character_radius);
}

void CharacterComponent::DrawDebugPrimitiveGUI()
{
    this->character_radius.DrawDebugGUI("clinde_push_radius");
}

#endif // _DEBUG