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

void CharacterComponent::Update(float elapsed_time)
{
    this->param.invincible_timer = (std::max)(this->param.invincible_timer - elapsed_time, 0.0f);   // ���G���ԍX�V
    this->param.old_hp = this->param.hp;
}

void CharacterComponent::ApplyDamage(int damege)
{
    if (damege <= 0) return;
    if (0.0f < this->param.invincible_timer) return;    // ���G���Ȃ�Damage���󂯂Ȃ�
    this->param.hp = (std::max)(this->param.hp - damege, 0);

    this->param.invincible_timer = this->param.max_invincible_timer;    // ���G���Ԑݒ�
}

bool CharacterComponent::IsAlive()
{
    return (0 < this->param.hp);    // �̗͂�0�ȏ�Ȃ琶��
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
    if (ImGui::InputFloat("Invincible Timer", &this->param.invincible_timer));
    if (ImGui::InputFloat("Max Invincible Timer", &this->param.max_invincible_timer));

    // �_���[�W�e�X�g
    ImGui::InputInt("Test Damage", &this->test_damage);
    if (ImGui::Button("Test ApplyDamage"))
    {
        ApplyDamage(this->test_damage);
    }

    bool is_alive = IsAlive();
    ImGui::Checkbox("Is Alive", &is_alive);
}

#endif // _DEBUG