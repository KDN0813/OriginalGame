#include <imgui.h>
#include "GravityComponent.h"
#include "Object/Object.h"
#include "Component/MovementComponent.h"

void GravityComponent::Update(float elapsed_time)
{
    auto owner = GetOwner();
    if (!owner) return;
    auto movement = owner->EnsureComponentValid<MovementComponent>(this->movement_Wptr);
    if (!movement) return;
    movement->AddAccelerationY(this->param.gravity);
}

#ifdef _DEBUG

void GravityComponent::DrawDebugGUI()
{
    ImGui::InputFloat("Gravity", &this->param.gravity);
    ImGui::Checkbox("Is Grounded", &this->param.is_grounded);
}

#endif // _DEBUG