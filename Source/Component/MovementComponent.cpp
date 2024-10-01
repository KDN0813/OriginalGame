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
            this->move_vec.x * this->speed * elapsed_time,
            this->move_vec.y * this->speed * elapsed_time,
            this->move_vec.z * this->speed * elapsed_time,
        };
        transform3D->AddPosition(movement);
    }
}

bool MovementComponent::IsMoveXZAxis()
{
    return (this->move_vec.x != 0.0f || this->move_vec.z != 0.0f);
}

#ifdef _DEBUG

void MovementComponent::DrawDebugGUI()
{
    ImGui::InputFloat3("velocity", &move_vec.x);
}

#endif _DEBUG