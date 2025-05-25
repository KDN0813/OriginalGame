#include "DebugCharacterComponent.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

#include "Object\Object.h"
#include "Component\ModelPartAnimationControlComponent.h"
#include "Component\InstancedModelWithAnimationComponent.h"
#include "Object\Constant\PlayerConstant.h"

void DebugCharacterComponent::Update(float elapsed_time)
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& animation = owner->GetComponent(this->animation_Wptr);
    if (!animation)return;

    // �ړ��A�j���Đ�
    if (GetKeyState('I') & 0x8000)
    {
        animation->PlayAnimation(PlayerConstant::ANIMATION::MOVE_FWD, true, blend_time);
    }
    // �ҋ@�A�j���Đ�
    else if (GetKeyState('O') & 0x8000)
    {
        animation->PlayAnimation(PlayerConstant::ANIMATION::IDLE, true, blend_time);
    }
    // �U���A�j���Đ�
    else if (GetKeyState('P') & 0x8000)
    {
        animation->PlayAnimation(PlayerConstant::ANIMATION::ATTACK01, true, blend_time);
    }
}

#ifdef _DEBUG
void DebugCharacterComponent::DrawDebugGUI()
{
    ImGui::InputFloat("blend_time", &this->blend_time);
}
#endif // _DEBUG

void DebugCharacterComponent_I::Update(float elapsed_time)
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& animation = owner->GetComponent(this->animation_Wptr);
    if (!animation)return;

    // �ړ��A�j���Đ�
    if (GetKeyState('I') & 0x8000)
    {
        animation->PlayAnimation(PlayerConstant::ANIMATION::MOVE_FWD, true, blend_time);
    }
    // �ҋ@�A�j���Đ�
    else if (GetKeyState('O') & 0x8000)
    {
        animation->PlayAnimation(PlayerConstant::ANIMATION::IDLE, true, blend_time);
    }
    // �U���A�j���Đ�
    else if (GetKeyState('P') & 0x8000)
    {
        animation->PlayAnimation(PlayerConstant::ANIMATION::ATTACK01, true, blend_time);
    }
}

#ifdef _DEBUG

void DebugCharacterComponent_I::DrawDebugGUI()
{
    ImGui::InputFloat("blend_time", &this->blend_time);
}

#endif // _DEBUG