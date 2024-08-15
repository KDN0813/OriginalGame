#include "InstancingModelComponent.h"

#include "Model/InstancingModelResourceManager.h"
#include "Model/ModelResourceManager.h"

InstancingModelComponent::InstancingModelComponent(ID3D11Device* device, const char* filename)
    :transform()
    , anime_frame()
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
