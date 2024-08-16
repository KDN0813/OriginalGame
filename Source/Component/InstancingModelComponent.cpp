#include <imgui.h>
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
    if (!this->anime_play)return;

    ++this->anime_frame;

    if (this->anime_frame >= this->instancing_model_resource->GetAnimationLengths()[this->anime_index])
    {
        if (this->anime_loop)
        {
            this->anime_frame = 0;
        }
        else
        {
            this->anime_frame = this->instancing_model_resource->GetAnimationLengths()[this->anime_index];
            this->anime_play = false;
        }
    }
}

void InstancingModelComponent::PlayAnimetion(int animeIndex, bool loop)
{
    if (animeIndex < 0 || animeIndex >= this->model_resource->GetAnimations().size()) return;

    this->anime_frame = 0;;
    this->anime_index = animeIndex;
    this->anime_loop = loop;
    this->anime_play = true;
}

const UINT InstancingModelComponent::GetAnimationStartOffset()
{
    // TODO(08/13) ƒAƒjƒ”Ô†‚ªˆÈã‚Èê‡ˆ—‚µ‚È‚¢‚æ‚¤‚É‚·‚é
    return this->instancing_model_resource->GetAnimationOffsets()[this->anime_index];
}

void InstancingModelComponent::DrawDebugGUI()
{
    //UINT anime_frame;
    //UINT anime_index;
    
    ImGui::Checkbox("anime_loop", &this->anime_loop);
    ImGui::Checkbox("anime_play", &this->anime_play);
}
