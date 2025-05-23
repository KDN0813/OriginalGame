#ifdef _DEBUG
#include "Debug\DebugManager.h"
#include "Debug/ImGuiHelper.h"
#endif // _DEBUG

#include "InstancedModelWithAnimationComponent.h"
#include "Graphics/Graphics.h"
#include "Object/Object.h"

#include "Model/InstancingModelResourceManager.h"
#include "Model/ModelResourceManager.h"

#include "Component/TransformComponent.h"

InstancedModelWithAnimationComponent::InstancedModelWithAnimationComponent(InstancedModelParam param, const char* filename)
    :param(param), default_param(param)
{
    ID3D11Device* device = nullptr;
    {
        Graphics::Instance graphics = Graphics::GetInstance();
        device = graphics->GetDevice();
    }

    if (ModelResourceManager::Instance model_resource_manager = ModelResourceManager::GetInstance(); model_resource_manager.Get())
    {
        this->model_resource = model_resource_manager->LoadModelResource(device, filename);
    }

    if (InstancingModelResourceManager::Instance instancing_model_resource_manager = InstancingModelResourceManager::GetInstance(); instancing_model_resource_manager.Get())
    {
        this->instancing_model_resource = instancing_model_resource_manager->LoadModelResource(device, filename);
    }

#ifdef _DEBUG
    this->is_draw_debug_primitive = false;

    this->model_filename = filename;
    
    // AABBの頂点描画用クラス初期化
    this->AABB_corners_vec.resize(this->model_resource->GetMeshes().size(), AABBCorners({ 1.0f,1.0f ,1.0f ,1.0f }, 0.1f));

    // アニメーションの名前を登録   
    for (const auto& animation : this->model_resource->GetAnimations())
    {
        this->animation_name_pool.emplace_back(animation.name);
    }
#endif // _DEBUG
}

void InstancedModelWithAnimationComponent::ReStart()
{
    param = default_param;
}

void InstancedModelWithAnimationComponent::Update(float elapsed_time)
{
    if (!this->param.anime_play)return;
    UpdateAnimation(elapsed_time);

#ifdef _DEBUG	// バウンディングボックス位置更新

    if (const auto& owner = GetOwner())
    {
        if (const auto& transform = owner->GetComponent(this->transform_Wptr))
        {
            MYMATRIX World_transform = transform->GetWorldTransform();

            for (size_t i = 0; i < this->AABB_corners_vec.size(); ++i)
            {
                DirectX::BoundingBox bounding_box;
                model_resource->GetDefaultBoundingBox(i).Transform(bounding_box, World_transform.GetMatrix());
                DirectX::XMFLOAT3 corners[8];
                bounding_box.GetCorners(corners);
                this->AABB_corners_vec[i].SetCenter(corners);
            }
        }
    }
#endif // _DEBUG
}

void InstancedModelWithAnimationComponent::PlayAnimation(int animeIndex, bool loop)
{
    if (animeIndex < 0 || animeIndex >= this->model_resource->GetAnimations().size()) return;

    // TODO ハイプロ用
    if (this->param.anime_index == animeIndex)return;

    this->param.current_animation_seconds = 0;;
    this->param.anime_index = animeIndex;
    this->param.anime_loop = loop;
    this->param.anime_play = true;
}

void InstancedModelWithAnimationComponent::PlayAnimation(const PlayAnimeParam& play_anime_param)
{
    this->param.current_animation_seconds = 0;;
    this->param.anime_index = static_cast<UINT>(play_anime_param.anime_index);
    this->param.anime_loop = play_anime_param.loop;
    this->param.anime_play = true;
}

void InstancedModelWithAnimationComponent::UpdateAnimation(float elapsed_time)
{
    const auto& owner = GetOwner();
    if (owner && owner->GetIsOffScreen()) return;

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

UINT InstancedModelWithAnimationComponent::GetAnimeFrame()
{
    const auto& animation = this->model_resource->GetAnimations()[this->param.anime_index];
    const UINT animation_frame_max = this->instancing_model_resource->GetAnimationLengths()[this->param.anime_index];
    const float animation_frame_max_float = static_cast<float>(animation_frame_max);
    const float animation_length = animation.seconds_length;

    return static_cast<UINT>(animation_frame_max_float * (this->param.current_animation_seconds / animation_length));
}

UINT InstancedModelWithAnimationComponent::GetAnimationStartOffset()
{
    return this->instancing_model_resource->GetAnimationOffsets()[this->param.anime_index];
}

int InstancedModelWithAnimationComponent::GetModelId()
{
    return this->instancing_model_resource->GetModelId();
}

std::vector<DirectX::BoundingBox> InstancedModelWithAnimationComponent::GetBoundingBoxs()
{
    std::vector<DirectX::BoundingBox> bounding_box_vec;

    if (const auto& owner = GetOwner())
    {
        if (const auto& transform = owner->GetComponent(this->transform_Wptr))
        {
            MYMATRIX World_transform = transform->GetWorldTransform();

            for (const auto& bounding_box : model_resource->GetDefaultBoundingBoxs())
            {
                DirectX::BoundingBox box;
                bounding_box.Transform(box, World_transform.GetMatrix());
                bounding_box_vec.emplace_back(box);
            }
        }
    }
    return bounding_box_vec;
}

#ifdef _DEBUG

void InstancedModelWithAnimationComponent::DrawDebugGUI()
{
    ImGui::Checkbox("Animation Play", &this->param.anime_play);

    DrawDebugAnimationGUI();

    ImGui::ColorEdit4("Base Color", &this->param.base_color.x);

    char buffer[1024];
    ::strncpy_s(buffer, sizeof(buffer), model_filename, sizeof(buffer));
    ImGui::InputText("Model FileName", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue);
}

void InstancedModelWithAnimationComponent::DrawDebugAnimationGUI()
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

void InstancedModelWithAnimationComponent::DrawDebugPrimitive()
{
    DebugManager::Instance debug_manager = DebugManager::GetInstance();
    DebugPrimitiveRenderer* debug_render = debug_manager->GetDebugPrimitiveRenderer();;
    
    for (auto& AABB_corners : AABB_corners_vec)
    {
        debug_render->DrawAABBCorners(AABB_corners);
    }
}

void InstancedModelWithAnimationComponent::DrawDebugPrimitiveGUI()
{
    size_t i = 0;
    for (auto& AABB_corners : AABB_corners_vec)
    {
        AABB_corners.DrawDebugGUI(("boudybox_point" + std::to_string(i)));
        ++i;
    }
}

#endif // _DEBUG