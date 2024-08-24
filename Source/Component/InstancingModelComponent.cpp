#include <imgui.h>
#include "InstancingModelComponent.h"
#include "Object/Object.h"

#include "Model/InstancingModelResourceManager.h"
#include "Model/ModelResourceManager.h"

#include "Component/TransformComponent.h"

InstancingModelComponent::InstancingModelComponent(ID3D11Device* device, const char* filename)
{
    this->instancing_model_resource =
        InstancingModelResourceManager::Instance()->LoadModelResource(device, filename);
    this->model_resource =
        ModelResourceManager::Instance()->LoadModelResource(device,filename);
}

void InstancingModelComponent::Update(float elapsedTime)
{
    if (!this->anime_play)return;

    this->anime_frame = (std::max)(static_cast<int>(this->anime_frame) + this->anime_speed, 0);

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
    return this->instancing_model_resource->GetAnimationOffsets()[this->anime_index];
}

#ifdef _DEBUG

void InstancingModelComponent::DrawDebugGUI()
{
    const int anime_index_max = static_cast<int>(this->model_resource->GetAnimations().size() - 1);
    int anime_index_int = static_cast<int>(this->anime_index);
    if (ImGui::SliderInt("anime_index", &anime_index_int, 0, anime_index_max))
    {
        this->anime_frame = 0;
        this->anime_index = static_cast<UINT>(anime_index_int);
    }
    const int anime_frame_max = static_cast<int>(this->instancing_model_resource->GetAnimationLengths()[anime_index_int]);
    int anime_frame_int = static_cast<int>(this->anime_frame);
    if (ImGui::SliderInt("anime_frame", &anime_frame_int, 0, anime_frame_max))
    {
        this->anime_frame = static_cast<UINT>(anime_frame_int);
    }
    ImGui::SliderInt("anime_speed", &this->anime_speed, 0, 10);

    ImGui::Checkbox("anime_loop", &this->anime_loop);
    ImGui::Checkbox("anime_play", &this->anime_play);
    if (ImGui::Button("anime_restart"))
    {
        this->anime_frame = 0;
        this->anime_play = true;
    }
}

#endif // _DEBUG