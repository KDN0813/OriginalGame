#include "DebugComponent.h"

#include "Object/Object.h"
#include "Component/TransformComponent.h"

void DebugComponent::Update(float elapsed_time)
{
    auto owner = GetOwner();
    if (!owner) return;
    auto transform = owner->GetComponent<Transform3DComponent>();
    if (!transform) return;

    transform->AddPosition(DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f));
}
