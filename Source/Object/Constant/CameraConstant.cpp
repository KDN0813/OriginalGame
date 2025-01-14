#include "CameraConstant.h"

#include "Object\Object.h"
#include "Camera\CameraManager.h"

#include "Component\CameraComponent.h"

const std::shared_ptr<Object>& CameraConstant::CreateDeathCamera(const std::shared_ptr<Object>& object)
{
	// CameraComponent
	if (CameraManager::Instance camera_manager = CameraManager::GetInstance(); camera_manager.Get())
	{
		object->AddComponent<CameraComponent>(camera_manager->GetCamera(CAMERA_TYPE::DEATH));
	}

	return object;
}
