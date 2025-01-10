#include "CharacterComponent.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG


CharacterComponent::CharacterComponent(CharacterParam param)
    :param(param), default_param(param)
{
}

void CharacterComponent::ReStart()
{
    this->param = this->default_param;
}

void CharacterComponent::ApplyDamage(int damege)
{
    this->param.hp = (std::min)((std::max)(this->param.hp - damege, 0), this->param.max_hp);
}

bool CharacterComponent::IsAlive()
{
    return (0 < this->param.hp);    // 体力が0以上なら生存
}

bool CharacterComponent::IsDamage()
{
    return (this->param.hp < this->param.old_hp);
}

#ifdef _DEBUG

void CharacterComponent::DrawDebugGUI()
{
    if (ImGui::InputInt("HP", &this->param.hp)) this->param.hp = (std::min)((std::max)(this->param.hp, 0), this->param.max_hp);
    ImGui::InputInt("Max HP", &this->param.max_hp);

    // ダメージテスト
    ImGui::InputInt("Test Damage", &this->test_damage);
    if (ImGui::Button("Test ApplyDamage"))
    {
        ApplyDamage(this->test_damage);
    }

    bool is_alive = IsAlive();
    ImGui::Checkbox("Is Alive", &is_alive);
}

#endif // _DEBUG