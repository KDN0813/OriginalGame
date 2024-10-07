#include "Debug/ImGuiHelper.h"
#include "InstancingModelComponent.h"
#include "Object/Object.h"

#include "Model/InstancingModelResourceManager.h"
#include "Model/ModelResourceManager.h"

#include "Component/TransformComponent.h"

AnimatedInstancedModelComponent::AnimatedInstancedModelComponent(ID3D11Device* device, const char* filename)
{
#ifdef _DEBUG
    this->model_filename = filename;
#endif // _DEBUG

    this->model_resource =
        ModelResourceManager::Instance()->LoadModelResource(device,filename);
    this->instancing_model_resource =
        InstancingModelResourceManager::Instance()->LoadModelResource(device, filename);

    // �A�j���X�e�[�g�̏����ݒ�
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

void AnimatedInstancedModelComponent::Update(float elapsed_time)
{
    UpdateAnimationState();
    if (!this->anime_play)return;
    UpdateAnimation(elapsed_time);
}

void AnimatedInstancedModelComponent::PlayAnimation(int animeIndex, bool loop)
{
    if (animeIndex < 0 || animeIndex >= this->model_resource->GetAnimations().size()) return;

    this->current_animation_seconds = 0;;
    this->anime_index = animeIndex;
    this->anime_loop = loop;
    this->anime_play = true;
}

void AnimatedInstancedModelComponent::PlayAnimation(const AnimeState& animation_info)
{
    this->current_animation_seconds = 0;;
    this->anime_index = animation_info.anime_index;
    this->anime_loop = animation_info.loop;
    this->anime_play = true;
}

void AnimatedInstancedModelComponent::UpdateAnimation(float elapsed_time)
{    
    const auto& animation = this->model_resource->GetAnimations()[this->anime_index];
    const UINT& animation_frame_max = this->instancing_model_resource->GetAnimationLengths()[this->anime_index];
    const float& animation_length = animation.seconds_length;
    
    this->current_animation_seconds += elapsed_time;

    if (this->current_animation_seconds >= animation_length)
    {
        if (this->anime_loop)
        {
            this->current_animation_seconds = 0.0f;
        }
        else
        {
            this->current_animation_seconds = animation_length;
            this->anime_play = false;
        }
    }
}

void AnimatedInstancedModelComponent::UpdateAnimationState()
{
#ifdef _DEBUG
    if (this->stop_anime_state_update) return;
#endif // DEBUG


    if (this->anime_index < 0) return;
    if (this->anime_state_pool.size() <= 0) return;

    for (auto& transition_info : this->anime_state_pool[this->anime_index].transition_info_pool)
    {
        if (PerformTransitionJudgement(transition_info->judgement.get()))
        {
            PlayAnimation(this->anime_state_pool[transition_info->next_anime_index]);
            break;
        }
    }
}

void AnimatedInstancedModelComponent::SetAnimationState(AnimeIndex anime_index, bool loop, float transition_ready_time)
{
    auto& anime_state = this->anime_state_pool[anime_index];
    anime_state.loop = loop;
    anime_state.transition_ready_time = transition_ready_time;
}

void AnimatedInstancedModelComponent::AddAnimationTransition(AnimeIndex anime_index, AnimeIndex transition_anime_index, std::unique_ptr<AnimeTransitionJudgementBase> judgement)
{
    auto& transition_info_pool = this->anime_state_pool[anime_index].transition_info_pool;
    // �J�ڂ���A�j���[�V�����X�e�[�g
    auto& transition_info = transition_info_pool.emplace_back();
    transition_info = std::make_unique<AnimeTransitionInfo>();
    transition_info->next_anime_index = transition_anime_index;
    transition_info->judgement = std::move(judgement);
}

bool AnimatedInstancedModelComponent::IsTransitionReady()
{
    // �Đ����łȂ���Ώ�������
    if (!this->anime_play) return true;

    auto& anime_state = this->anime_state_pool[this->anime_index];

    if (anime_state.transition_ready_time >= 0 &&
        anime_state.transition_ready_time <= this->current_animation_seconds) return true;

    return false;
}

bool AnimatedInstancedModelComponent::PerformTransitionJudgement(AnimeTransitionJudgementBase* judgemen)
{
    if (!judgemen) return false;
#ifdef _DEBUG
    if (!judgemen->GetIsActive()) return false;
#endif // _DEBUG
    auto owner = GetOwner();
    if (!owner) return false;

    // �J�ڏ�����҂t���O���I���̏ꍇ�A�J�ڂ̏����������܂őҋ@����
    if (judgemen->GetRequireTransitionReady() && !IsTransitionReady()) return false;

    return judgemen->GetShouldReversey() ? !judgemen->CheckTransitionCondition() : judgemen->CheckTransitionCondition();
}

const UINT AnimatedInstancedModelComponent::GetAnimeFrame()
{
    const auto& animation = this->model_resource->GetAnimations()[this->anime_index];
    const UINT& animation_frame_max = this->instancing_model_resource->GetAnimationLengths()[this->anime_index];
    const float animation_frame_max_f = static_cast<float>(animation_frame_max);
    const float& animation_length = animation.seconds_length;

    return static_cast<UINT>(animation_frame_max_f * (this->current_animation_seconds / animation_length));
}

const UINT& AnimatedInstancedModelComponent::GetAnimationStartOffset()
{
    return this->instancing_model_resource->GetAnimationOffsets()[this->anime_index];
}

const int& AnimatedInstancedModelComponent::GetModelId()
{
    return this->instancing_model_resource->GetModelId();
}

#ifdef _DEBUG

void AnimatedInstancedModelComponent::DrawDebugGUI()
{
    ImGui::Checkbox("Animation Play", &this->anime_play);

    DrawDebugAnimationGUI();

    char buffer[1024];
    ::strncpy_s(buffer, sizeof(buffer), model_filename, sizeof(buffer));
    ImGui::InputText("Model FileName", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue);
}

void AnimatedInstancedModelComponent::DrawDebugAnimationGUI()
{
    int anime_index_int = static_cast<int>(this->anime_index);
    if (anime_index_int < 0) return;

    const auto& animation = this->model_resource->GetAnimations()[anime_index_int];

    std::string play_anime_name = this->animation_name_pool[anime_index_int];

    // ���݂̃t���[�����\��
    ImGui::SliderFloat("Current Animation Seconds", &this->current_animation_seconds, 0.0f, animation.seconds_length);
    int anime_frame_i = GetAnimeFrame();
    ImGui::InputInt("Anime Frame",&anime_frame_i);
    if (ImGuiComboUI("Animation", play_anime_name, this->animation_name_pool, anime_index_int))
    {
        auto& anime_state = this->anime_state_pool[anime_index_int];
        PlayAnimation(anime_state);
    }
    ImGui::Checkbox("Animation Loop Flag", &this->anime_loop);

    if (this->is_draw_deletail) DrawDetail();
    else this->is_draw_deletail = ImGui::Button("Draw Animation Deletail");
}

void AnimatedInstancedModelComponent::DrawDetail()
{
    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);

    ImGui::Begin("Animation Detail", &is_draw_deletail, ImGuiWindowFlags_None);

    std::string select_anime_name = this->animation_name_pool[this->select_animation_index];
    ImGuiComboUI("Select Animation", select_anime_name, this->animation_name_pool, this->select_animation_index);

    auto& selct_anime_state = this->anime_state_pool[this->select_animation_index];

    // �Đ�
    if (ImGui::Button("Play Animation"))
    {
        PlayAnimation(selct_anime_state);
    }

    // �A�j���[�V�������
    if (ImGui::CollapsingHeader("Animation Info", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Loop", &selct_anime_state.loop);
        ImGui::InputFloat("Transition Ready Time", &selct_anime_state.transition_ready_time);

        // �Đ��I������
        float seconds_length = model_resource->GetAnimations()[this->select_animation_index].seconds_length;
        ImGui::InputFloat("Seconds Length", &seconds_length);
        int seconds_max_frame = static_cast<int>(instancing_model_resource->GetAnimationLengths()[this->select_animation_index]);
        ImGui::InputInt("Seconds Max Frame", &seconds_max_frame);
    }
    // �J�ڏ��\��
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

            // ��A�N�e�B�u�Ȃ�D�F�ɂ���
            if (!is_judgement_active) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));// �D�F

            auto& transition_anime_state = this->anime_state_pool[transition_info->next_anime_index];
            if (ImGui::CollapsingHeader(transition_anime_state.name.c_str(), ImGuiTreeNodeFlags_None))
            {
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