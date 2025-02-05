#include "SpinAttackTimeUIComponent.h"

#include "Object\Object.h"

#include "Component\PlayerComponent.h"
#include "Component\SpriteComponent.h"

void SpinAttackTimeUIComponent::Update(float elapsed_time)
{
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& sprite = owner->GetComponent(this->sprite_Wptr);
    if (!sprite)return;

    const auto parent = owner->GetParent();
    if (!parent) return;
    const auto& player_component = parent->GetComponent(this->player_Wptr);
    if (!player_component) return;

    // ポイントUIのサイズ更新
    sprite->SetDisplaySizeX(player_component->GetSpecialPoint() / player_component->GetSpecialPointMax());
}
