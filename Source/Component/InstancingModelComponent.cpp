#include "Debug/ImGuiHelper.h"
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
    DrawDebugAnimationGUI();

    char buffer[1024];
    ::strncpy_s(buffer, sizeof(buffer), model_filename, sizeof(buffer));
    ImGui::InputText("Model FileName", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue);
}

void InstancingModelComponent::DrawDebugAnimationGUI()
{
    ImGui::Checkbox("Animation Play", &this->anime_play);

    ImGui::SameLine();

    // アニメーションの停止中なら灰色にする
    if (!this->anime_play) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));// 灰色

    if (ImGui::CollapsingHeader("AnimationParam", ImGuiTreeNodeFlags_None))
    {
        int anime_index_int = static_cast<int>(this->anime_index);
        if (anime_index_int < 0) return;

        const auto& animation = this->model_resource->GetAnimations()[anime_index_int];

        std::string play_anime_name = this->animation_name_pool[anime_index_int];
        ImGui::Checkbox("Stop Anime State Update", &this->stop_anime_state_update);
        
        // 現在のフレーム数表示
        const int anime_frame_max = static_cast<int>(this->instancing_model_resource->GetAnimationLengths()[anime_index_int]);
        int anime_frame_int = static_cast<int>(this->anime_frame);
        if (ImGui::SliderInt("Anime Frame", &anime_frame_int, 0, anime_frame_max))
        {
            this->anime_frame = static_cast<UINT>(anime_frame_int);
        }
        if (ImGuiComboUI("Animation", play_anime_name, this->animation_name_pool, anime_index_int))
        {
            auto& anime_state = this->anime_state_pool[anime_index_int];
            PlayAnimation(anime_state);
        }
        ImGui::Checkbox("Animation Loop Flag", &this->anime_loop);

        if (this->is_draw_deletail) DrawDetail();
        else this->is_draw_deletail = ImGui::Button("Draw Animation Deletail");

    }
    if (!this->anime_play) ImGui::PopStyleColor();
}

void InstancingModelComponent::DrawDetail()
{
    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);

    ImGui::Begin("Animation Detail", &is_draw_deletail, ImGuiWindowFlags_None);

    std::string select_anime_name = this->animation_name_pool[this->select_animation_index];
    ImGuiComboUI("Select Animation", select_anime_name, this->animation_name_pool, this->select_animation_index);

    auto& selct_anime_state = this->anime_state_pool[this->select_animation_index];

    // 再生
    if (ImGui::Button("Play Animation"))
    {
        PlayAnimation(selct_anime_state);
    }

    // アニメーション情報
    if (ImGui::CollapsingHeader("Animation Info", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Loop", &selct_anime_state.loop);
        ImGui::InputFloat("Transition Ready Time", &selct_anime_state.transition_ready_time);

        // 再生終了時間
        const auto& anime = this->model_resource->GetAnimations()[this->select_animation_index];
        float seconds_length = anime.seconds_length;
        ImGui::InputFloat("Seconds Length", &seconds_length);
    }
    // 遷移情報表示
    if (ImGui::CollapsingHeader("Transition Info", ImGuiTreeNodeFlags_DefaultOpen))
    {

        int transition_id = 0;
        for (auto& transition_info : selct_anime_state.transition_info_pool)
        {
            bool is_judgement_active = transition_info->judgement->GetIsActive();

            std::string label = "##" + std::to_string(transition_id);
            if (ImGui::Checkbox(label.c_str(), &is_judgement_active))
            {
                transition_info->judgement->SetIsActive(is_judgement_active);
            }

            ImGui::SameLine();

            // 非アクティブなら灰色にする
            if (!is_judgement_active) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));// 灰色

            auto& transition_anime_state = this->anime_state_pool[transition_info->next_anime_index];
            if (ImGui::CollapsingHeader(transition_anime_state.name.c_str(), ImGuiTreeNodeFlags_None))
            {
                ImGui::InputFloat("Blend Time", &transition_info->blend_time);
                std::string judgement_name = "Judgement:";
                judgement_name += transition_info->judgement->GetName();
                ImGui::Text(judgement_name.c_str());
                transition_info->judgement->DrawCommonDebugGUI(transition_id);
                transition_info->judgement->DrawDebugGUI(transition_id);
            }

            ++transition_id;
            if (!is_judgement_active) ImGui::PopStyleColor();
        }
    }

    ImGui::End();
}

#endif // _DEBUG