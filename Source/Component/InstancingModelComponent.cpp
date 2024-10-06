#include <imgui.h>
#include "InstancingModelComponent.h"
#include "Object/Object.h"

#include "Model/InstancingModelResourceManager.h"
#include "Model/ModelResourceManager.h"

#include "Component/TransformComponent.h"

InstancingModelComponent::InstancingModelComponent(ID3D11Device* device, const char* filename)
{
#ifdef _DEBUG
    this->model_filename = filename;
#endif // _DEBUG

    this->instancing_model_resource =
        InstancingModelResourceManager::Instance()->LoadModelResource(device, filename);
    this->model_resource =
        ModelResourceManager::Instance()->LoadModelResource(device,filename);

    // アニメステートの初期設定
    {
        this->anime_state_pool.clear();
        for (AnimeIndex index = 0; index < this->model_resource->GetAnimations().size(); ++index)
        {
            const auto& animation = this->model_resource->GetAnimations()[index];
            auto& anime_state = this->anime_state_pool.emplace_back();
            anime_state.anime_index = index;
            anime_state.name = animation.name;
#ifdef _DEBUG
            this->animation_name_pool.emplace_back(animation.name);
#endif // _DEBUG
        }
    }
}

void InstancingModelComponent::Update(float elapsed_time)
{
    if (!this->anime_play)return;
    
    UpdateAnimationState();
    UpdateAnimation();
}

void InstancingModelComponent::PlayAnimation(int animeIndex, bool loop)
{
    if (animeIndex < 0 || animeIndex >= this->model_resource->GetAnimations().size()) return;

    this->anime_frame = 0;;
    this->anime_index = animeIndex;
    this->anime_loop = loop;
    this->anime_play = true;
}

void InstancingModelComponent::PlayAnimation(const AnimeState& animation_info)
{
    this->anime_frame = 0;;
    this->anime_index = animation_info.anime_index;
    this->anime_loop = animation_info.loop;
    this->anime_play = true;
}

void InstancingModelComponent::UpdateAnimation()
{
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

void InstancingModelComponent::UpdateAnimationState()
{
#ifdef _DEBUG
    if (this->stop_anime) return;
    if (this->stop_anime_state_update) return;
#endif // DEBUG


    if (this->anime_index < 0) return;
    if (this->anime_state_pool.size() <= 0) return;

    for (auto& transition_info : this->anime_state_pool[this->anime_index].transition_info_pool)
    {
        if (transition_info->judgement->PerformTransitionJudgement())
        {
            PlayAnimation(this->anime_state_pool[transition_info->next_anime_index]);
            break;
        }
    }
}

void InstancingModelComponent::SetAnimationState(AnimeIndex anime_index, bool loop, float transition_ready_time)
{
    auto& anime_state = this->anime_state_pool[anime_index];
    anime_state.loop = loop;
    anime_state.transition_ready_time = transition_ready_time;
}

void InstancingModelComponent::AddAnimationTransition(AnimeIndex anime_index, AnimeIndex transition_anime_index, std::unique_ptr<AnimeTransitionJudgementBase> judgement, float blend_time)
{
    auto& transition_info_pool = this->anime_state_pool[anime_index].transition_info_pool;
    // 遷移するアニメーションステート
    auto& transition_info = transition_info_pool.emplace_back();
    transition_info = std::make_unique<AnimeTransitionInfo>();
    transition_info->next_anime_index = transition_anime_index;
    transition_info->judgement = std::move(judgement);
    transition_info->blend_time = blend_time;
}

const UINT InstancingModelComponent::GetAnimationStartOffset()
{
    return this->instancing_model_resource->GetAnimationOffsets()[this->anime_index];
}

const int InstancingModelComponent::GetModelId()
{
    return this->instancing_model_resource->GetModelId();
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

    char buffer[1024];
    ::strncpy_s(buffer, sizeof(buffer), model_filename, sizeof(buffer));
    ImGui::InputText("Model FileName", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue);
}

#endif // _DEBUG