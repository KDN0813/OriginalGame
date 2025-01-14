#include "LoadingIconStateDerived.h"
#include "Object\Object.h"

#include "Component\Transform2DComponent.h"

const MyHash LoadingIconAnimationState::STATE_NAME = MyHash("LoadingIconAnimationState");
LoadingIconAnimationState::LoadingIconAnimationState()
    :State(LoadingIconAnimationState::STATE_NAME)
{
}

void LoadingIconAnimationState::Start()
{
    this->timer = 0.0f;
}

void LoadingIconAnimationState::Update(float elapsed_time)
{
    if (const auto& onwer = GetOwner())
    {
        if (const auto& transform = onwer->GetComponent(this->transform_Wprt))
        {
            transform->SetLocalAngle(transform->GetLocalAngle() + ROTATION_SPEED);
        }
    }
}
