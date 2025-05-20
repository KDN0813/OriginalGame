#include "DebugComponent.h"

#ifdef _DEBUG
#include "Debug/DebugManager.h"
#endif // DEBUG


#include "Object/Object.h"
#include "Component/TransformComponent.h"

void DebugComponent::Update(float elapsed_time)
{
    auto owner = GetOwner();
    if (!owner) return;
    auto transform = owner->GetComponent<Transform3DComponent>();
    if (!transform) return;

    float spped = 1.0f * elapsed_time;

    transform->AddLocalPosition(DirectX::XMFLOAT3(spped, 0.0f, 0.0f));
}

#ifdef _DEBUG

void DebugComponent::DrawDebugPrimitive()
{
    auto owner = GetOwner();
    if (!owner) return;
    auto transform = owner->GetComponent<Transform3DComponent>();
    if (!transform) return;

    DebugManager::Instance debug_manager = DebugManager::GetInstance();
    DebugPrimitiveRenderer* debug_primitive_render = debug_manager->GetDebugPrimitiveRenderer();
    if (!debug_primitive_render) return;
    debug_primitive_render->DrawSphere(transform->GetWorldPosition(), 1.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
}

void DebugComponent::DrawDebugPrimitiveGUI()
{
    auto owner = GetOwner();
    if (!owner) return;
    auto transform = owner->GetComponent<Transform3DComponent>();
    if (!transform) return;
    DirectX::XMFLOAT3 w = transform->GetWorldPosition();
    ImGui::InputFloat3("GetWorldPosition", &w.x);
}

#endif // DEBUG