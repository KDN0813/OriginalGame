#include <imgui.h>
#include "GravityComponent.h"
#include "Object/Object.h"
#include "Component/MovementComponent.h"

void GravityComponent::Update(float elapsed_time)
{
    //if (this->is_grounded) return;

    //auto owner = GetOwner();
    //if (!owner) return;
    //auto movement = owner->EnsureComponentValid<MovementComponent>(this->movement_Wptr);
    //if (!movement) return;

    //DirectX::XMFLOAT3 pos = transform->GetPosition();
    //pos.y += this->gravity * elapsed_time;
    //transform->SetPosition(pos);
}

#ifdef _DEBUG

void GravityComponent::DrawDebugGUI()
{
    ImGui::InputFloat("Gravity", &this->gravity);
    ImGui::Checkbox("Is Grounded", &this->is_grounded);
}

#endif // _DEBUG