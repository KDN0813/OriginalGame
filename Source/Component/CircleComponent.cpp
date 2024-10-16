#include "CircleComponent.h"
#include "Object/Object.h"

#include "Component/TransformComponent.h"

void CircleComponent::Start()
{
}

void CircleComponent::End()
{
}

void CircleComponent::Update(float elapsed_time)
{
}

CircleParam CircleComponent::GetCircleParam()
{
    auto owner = GetOwner();
    if (!owner) return CircleParam(DirectX::XMFLOAT2(), this->radius);
    auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr);
    if(!transform) return CircleParam(DirectX::XMFLOAT2(), this->radius);

    DirectX::XMFLOAT3 world_pos = transform->GetWorldPosition();
    return CircleParam(DirectX::XMFLOAT2(world_pos.x, world_pos.z), this->radius);
}
