#include "Debug/ImGuiHelper.h"
#include "AnimatedControlInstancedModelComponent.h"
#include "Graphics/Graphics.h"
#include "Object/Object.h"

#include "Model/InstancingModelResourceManager.h"
#include "Model/ModelResourceManager.h"

#include "Component/TransformComponent.h"

AnimatedControlInstancedModelComponent::AnimatedControlInstancedModelComponent(InstancedModelParam param, const char* filename)
    :param(param), default_param(param)
{
#ifdef _DEBUG
    this->model_filename = filename;
#endif // _DEBUG
    Graphics* graphics = Graphics::Instance();
    std::lock_guard<std::mutex> lock(graphics->GetInstanceMutex());
    ID3D11Device* device = graphics->GetDevice();

    this->model_resource =
        ModelResourceManager::Instance()->LoadModelResource(device, filename);
    this->instancing_model_resource =
        InstancingModelResourceManager::Instance()->LoadModelResource(device, filename);

#ifdef _DEBUG
    // アニメーションの名前を登録   
    for (const auto& animation : this->model_resource->GetAnimations())
    {
        this->animation_name_pool.emplace_back(animation.name);
    }
#endif // _DEBUG
}

void AnimatedControlInstancedModelComponent::ReStart()
{
    param = default_param;
}

void AnimatedControlInstancedModelComponent::Update(float elapsed_time)
{
    if (!this->param.anime_play)return;
    UpdateAnimation(elapsed_time);
}

void AnimatedControlInstancedModelComponent::PlayAnimation(int animeIndex, bool loop)
{
    if (animeIndex < 0 || animeIndex >= this->model_resource->GetAnimations().size()) return;

    this->param.current_animation_seconds = 0;;
    this->param.anime_index = animeIndex;
    this->param.anime_loop = loop;
    this->param.anime_play = true;
}

void AnimatedControlInstancedModelComponent::PlayAnimation(const PlayAnimeParam& play_anime_param)
{
    this->param.current_animation_seconds = 0;;
    this->param.anime_index = static_cast<UINT>(play_anime_param.anime_index);
    this->param.anime_loop = play_anime_param.loop;
    this->param.anime_play = true;
}

void AnimatedControlInstancedModelComponent::UpdateAnimation(float elapsed_time)
{
    const auto& animation = this->model_resource->GetAnimations()[this->param.anime_index];
    const UINT& animation_frame_max = this->instancing_model_resource->GetAnimationLengths()[this->param.anime_index];
    const float& animation_length = animation.seconds_length;

    this->param.current_animation_seconds += elapsed_time;

    if (this->param.current_animation_seconds >= animation_length)
    {
        if (this->param.anime_loop)
        {
            this->param.current_animation_seconds = 0.0f;
        }
        else
        {
            this->param.current_animation_seconds = animation_length;
            this->param.anime_play = false;
        }
    }
}

UINT AnimatedControlInstancedModelComponent::GetAnimeFrame()
{
    const auto& animation = this->model_resource->GetAnimations()[this->param.anime_index];
    const UINT animation_frame_max = this->instancing_model_resource->GetAnimationLengths()[this->param.anime_index];
    const float animation_frame_max_float = static_cast<float>(animation_frame_max);
    const float animation_length = animation.seconds_length;

    return static_cast<UINT>(animation_frame_max_float * (this->param.current_animation_seconds / animation_length));
}

UINT AnimatedControlInstancedModelComponent::GetAnimationStartOffset()
{
    return this->instancing_model_resource->GetAnimationOffsets()[this->param.anime_index];
}

int AnimatedControlInstancedModelComponent::GetModelId()
{
    return this->instancing_model_resource->GetModelId();
}

#ifdef _DEBUG

void AnimatedControlInstancedModelComponent::DrawDebugGUI()
{
    ImGui::Checkbox("Animation Play", &this->param.anime_play);

    DrawDebugAnimationGUI();

    char buffer[1024];
    ::strncpy_s(buffer, sizeof(buffer), model_filename, sizeof(buffer));
    ImGui::InputText("Model FileName", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue);
}

void AnimatedControlInstancedModelComponent::DrawDebugAnimationGUI()
{
    int anime_index_int = static_cast<int>(this->param.anime_index);
    if (anime_index_int < 0) return;

    const auto& animation = this->model_resource->GetAnimations()[anime_index_int];

    std::string play_anime_name = this->animation_name_pool[anime_index_int];

    // 現在のフレーム数表示
    ImGui::SliderFloat("Current Animation Seconds", &this->param.current_animation_seconds, 0.0f, animation.seconds_length);
    int anime_frame_i = GetAnimeFrame();
    ImGui::InputInt("Anime Frame", &anime_frame_i);
    if (ImGui::ComboUI("Animation", play_anime_name, this->animation_name_pool, anime_index_int))
    {
        PlayAnimation(anime_index_int);
    }
    ImGui::Checkbox("Animation Loop Flag", &this->param.anime_loop);
}

#endif // _DEBUG