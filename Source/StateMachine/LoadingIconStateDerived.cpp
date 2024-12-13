#include "LoadingIconStateDerived.h"
#include "Object\Object.h"

#include "Component\SpriteComponent.h"

const MyHash LoadingIconAnimationState::STATE_NAME = MyHash("LoadingIconAnimationState");
LoadingIconAnimationState::LoadingIconAnimationState()
    :State(LoadingIconAnimationState::STATE_NAME)
{
}

void LoadingIconAnimationState::Staet()
{
    this->timer = 0.0f;
}

void LoadingIconAnimationState::Update(float elapsed_time)
{
    if (const auto& onwer = GetOwner())
    {
        if (const auto& sprite = onwer->EnsureComponentValid(this->sprite_Wprt))
        {
            sprite->SetAngle(sprite->GetAngle() + ROTATION_SPEED);
        }
    }
}
