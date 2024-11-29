#include "LoadingIconStateDerived.h"
#include "Object\Object.h"

#include "Component\SpriteComponent.h"

LoadingIconAnimationState::LoadingIconAnimationState()
    :State("LoadingIconAnimationState")
{
}

void LoadingIconAnimationState::Staet()
{
    this->timer = 0.0f;
}

void LoadingIconAnimationState::Update(float elapsed_time)
{
    if(0.1f <= this->timer)

    if (const auto& onwer = GetOwner())
    {
        if (const auto& sprite = onwer->EnsureComponentValid(this->sprite_Wprt))
        {
            sprite->SetIsActive(!sprite->GetIsActive());
            this->timer -= 0.2f;
        }
    }
    this->timer += elapsed_time;
}
