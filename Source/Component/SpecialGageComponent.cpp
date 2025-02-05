#include "SpecialGageComponent.h"

#include "Object\Object.h"
#include "Object\GameObject.h"
#include "Object\Constant\UIConstant.h"

#include "Component\SpriteComponent.h"
#include "Component\PlayerComponent.h"

void SpecialGageComponent::Start()
{
}

void SpecialGageComponent::Update(float elapsed_time)
{
    if (const auto owner = GetOwner())
    {
        if (const auto& sprite = owner->GetComponent(this->sprite_Wptr))
        {
            sprite->SetDisplaySizeX(CalculateSpecialPointWidth());
        }
    }
}

float SpecialGageComponent::CalculateSpecialPointWidth()
{
    if (GameObject::Instance game_object = GameObject::GetInstance(); game_object.Get())
    {
        if (const auto player = game_object->GetPlayer())
        {
            if (const auto& player_component = player->GetComponent(this->player_Wptr))
            {
                return player_component->GetSpecialPoint() / player_component->GetSpecialPointMax();
            }
        }
    }
    return 1.0f;
}