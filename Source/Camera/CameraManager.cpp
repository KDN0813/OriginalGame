#include "CameraManager.h"

#include "Component/CameraComponent.h"

void CameraManager::AddCamera(CameraComponent* camera)
{
}

void CameraManager::RemoveCamera(CameraComponent* camera)
{
    if (camera == this->main_camera) return;

    auto it = std::find(this->camera_vector.begin(), this->camera_vector.end(), camera);
    if (it != camera_vector.end())
    {
        this->camera_vector.erase(it);
    }
}

void CameraManager::SetMainCamera(CameraComponent* camera)
{
    this->main_camera = camera;
}
