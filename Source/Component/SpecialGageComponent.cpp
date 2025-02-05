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
    const auto& owner = GetOwner();
    if (!owner)return;
    const auto& sprite = owner->GetComponent(this->sprite_Wptr);
    if (!sprite)return;

    GameObject::Instance game_object = GameObject::GetInstance();
    const auto player = game_object->GetPlayer();
    if (!player) return;
    const auto& player_component = player->GetComponent(this->player_Wptr);
    if (!player_component) return;

    // ポイントUIのサイズ更新
    sprite->SetDisplaySizeX(player_component->GetSpecialPoint() / player_component->GetSpecialPointMax());

    // ゲージの更新
    {
        // 一度非アクティブにする
        for (int i = 0; i < player_component->GetGageCountMax(); ++i)
        {
            const auto& object = owner->FindChildObject(MyHash(UIConstant::SPIN_SPECIAL_GAGE_OBJECT_NAME.GetString() + std::to_string(i)));
            if (!object) continue;
            object->SetIsActive(false);
        }
        // ゲージの数だけアクティブ化する
        for (int i = 0; i < player_component->GetGageCount(); ++i)
        {
            const auto& object = owner->FindChildObject(MyHash(UIConstant::SPIN_SPECIAL_GAGE_OBJECT_NAME.GetString() + std::to_string(i)));
            if (!object) continue;
            object->SetIsActive(true);
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