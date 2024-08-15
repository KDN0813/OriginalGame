#include "InstancingModelComponent.h"
#include "Object/Object.h"

#include "Model/InstancingModelResourceManager.h"
#include "Model/ModelResourceManager.h"

#include "Component/TransformComponent.h"

InstancingModelComponent::InstancingModelComponent(ID3D11Device* device, const char* filename)
    : anime_frame()
    , anime_index()
    , anime_loop()
    , anime_play()

{
    this->instancing_model_resource =
        InstancingModelResourceManager::Instance()->LoadModelResource(device, filename);
    this->model_resource =
        ModelResourceManager::Instance()->LoadModelResource(filename);
}

void InstancingModelComponent::Update(float elapsedTime)
{
}

void InstancingModelComponent::PlayAnimetion(int animeIndex, bool loop)
{
}

const InstanceData InstancingModelComponent::GetInstanceData()
{
    DirectX::XMFLOAT4X4 transform{};

    auto owner = GetOwner();
    if (auto transform3D = owner->GetComponent<Transform3DComponent>(this->transform_Wptr))
    {
        transform = transform3D->GetTransform();
    }

    InstanceData instance_data
    {
        this->anime_frame,
         this->instancing_model_resource->GetAnimationOffsets()[this->anime_index],
         transform
    };

    return instance_data;
}
