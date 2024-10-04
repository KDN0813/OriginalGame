#include <imgui.h>
#include "GravityComponent.h"
#include "Object/Object.h"
#include "Component/TransformComponent.h"

void GravityComponent::Update(float elapsed_time)
{
    if (this->is_grounded) return;

    auto owner = GetOwner();
    if (!owner) return;
    auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr);
    if (!transform) return;

    DirectX::XMFLOAT3 pos = transform->GetPosition();
    pos.y += this->gravity * elapsed_time;
    transform->SetPosition(pos);
}

#ifdef _DEBUG

void GravityComponent::DrawDebugGUI()
{
    ImGui::InputFloat("Gravity", &this->gravity);
    ImGui::Checkbox("Is Grounded", &this->is_grounded);
}

#endif // _DEBUG