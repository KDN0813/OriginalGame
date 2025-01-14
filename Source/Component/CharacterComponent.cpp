#include "CharacterComponent.h"
#ifdef _DEBUG
#include <imgui.h>
#include <string>
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
    if (this->param.invincible_flag) return;            // ���G���Ȃ�Damage���󂯂Ȃ�
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
    std::string is_invincible_text = "Invincibility:";
    is_invincible_text +=
        (this->param.invincible_flag || 0.0f < this->param.invincible_timer) ?
        "Active" : "Not Active";
    ImGui::Text(is_invincible_text.c_str());
    ImGui::Checkbox("Invincible Flag", &this->param.invincible_flag);
    if (ImGui::InputInt("HP", &this->param.hp)) this->param.hp = (std::min)((std::max)(this->param.hp, 0), this->param.max_hp);
    ImGui::InputInt("Max HP", &this->param.max_hp);
    ImGui::InputFloat("Invincible Timer", &this->param.invincible_timer);
    ImGui::InputFloat("Max Invincible Timer", &this->param.max_invincible_timer);

    // �f�o�b�O�p�_���[�W�{�^��
    ImGui::InputInt("Test Damage", &this->test_damage);
    if (ImGui::Button("Test ApplyDamage"))
    {
        ApplyDamage(this->test_damage);
    }

    // �f�o�b�O�p���S�{�^��
    ImGui::InputInt("Test Damage", &this->test_damage);
    if (ImGui::Button("Test Death"))
    {
        this->param.hp = 0;
    }

    bool is_alive = IsAlive();
    ImGui::Checkbox("Is Alive", &is_alive);
}

#endif // _DEBUG