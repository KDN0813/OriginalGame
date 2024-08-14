#include <imgui.h>
#include "MovementComponent.h"
#include "Object/Object.h"

#include "Component/TransformComponent.h"

void MovementComponent::Update(float elapsed_time)
{
    auto owner = GetOwner();

    if (auto transform3D = owner->GetComponent(transform3D_Wptr))
    {
        // ˆÚ“®—Ê
        DirectX::XMFLOAT3 movement =
        {
            this->velocity.x * elapsed_time,
            this->velocity.y * elapsed_time,
            this->velocity.z * elapsed_time,
        };
        transform3D->AddPosition(movement);
    }
}

#ifdef _DEBUG

void MovementComponent::DrawDebugGUI()
{
    ImGui::InputFloat3("velocity", &velocity.x);
}

#endif _DEBUG