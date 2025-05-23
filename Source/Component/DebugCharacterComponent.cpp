#include "DebugCharacterComponent.h"

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
        animation->PlayAnimation(PlayerConstant::ANIMATION::MOVE_FWD, true, 0.2f);
    }
    // �ҋ@�A�j���Đ�
    else if (GetKeyState('O') & 0x8000)
    {
        animation->PlayAnimation(PlayerConstant::ANIMATION::IDLE, true, 0.2f);
    }
    // �U���A�j���Đ�
    else if (GetKeyState('P') & 0x8000)
    {
        animation->PlayAnimation(PlayerConstant::ANIMATION::ATTACK01, true, 0.2f);
    }
}

void DebugCharacterComponent_I::Update(float elapsed_time)
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& animation = owner->GetComponent(this->animation_Wptr);
    if (!animation)return;

    // �ړ��A�j���Đ�
    if (GetKeyState('I') & 0x8000)
    {
        animation->PlayAnimation(PlayerConstant::ANIMATION::MOVE_FWD, true);
    }
    // �ҋ@�A�j���Đ�
    else if (!this->is_on_key && GetKeyState('O') & 0x8000)
    {
        animation->PlayAnimation(PlayerConstant::ANIMATION::IDLE, true);
    }
    // �U���A�j���Đ�
    else if (!this->is_on_key && GetKeyState('P') & 0x8000)
    {
        animation->PlayAnimation(PlayerConstant::ANIMATION::ATTACK01, true);
    }
}
