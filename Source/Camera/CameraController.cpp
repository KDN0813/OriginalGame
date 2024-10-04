#include "CameraController.h"
#include "Object/Object.h"
#include"Component/CameraComponent.h"

std::shared_ptr<CameraComponent> CameraControllerBase::GetCamera()
{
    auto owner = this->owner_Wptr.lock();
    if (!owner) return nullptr;
    return owner->EnsureComponentValid<CameraComponent>(this->camera_Wptr);
}
