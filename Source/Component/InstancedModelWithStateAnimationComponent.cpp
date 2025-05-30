#include "Debug/ImGuiHelper.h"
#include "InstancedModelWithStateAnimationComponent.h"
#include "Graphics/Graphics.h"
#include "Object/Object.h"

#include "Model/InstancingModelResourceManager.h"
#include "Model/ModelResourceManager.h"

#include "Component/TransformComponent.h"

InstancedModelWithStateAnimationComponent::InstancedModelWithStateAnimationComponent(InstancedModelParam param, const char* filename)
    :InstancedModelWithAnimationComponent(param, filename)
{
    // アニメステートの初期設定
    {
        this->anime_state_pool.clear();
        for (AnimeIndex index = 0; index < this->model_resource->GetAnimations().size(); ++index)
        {
            const auto& animation = this->model_resource->GetAnimations()[index];
            auto& anime_state = this->anime_state_pool.emplace_back();
            anime_state.anime_index = index;
            anime_state.name = animation.name;
        }
    }
}

void InstancedModelWithStateAnimationComponent::ReStart()
{
    param = default_param;
}

void InstancedModelWithStateAnimationComponent::Update(float elapsed_time)
{
    UpdateAnimationState();
    if (!this->param.anime_play)return;
    UpdateAnimation(elapsed_time);
}

void InstancedModelWithStateAnimationComponent::PlayAnimation(const AnimeState& animation_info)
{
    this->param.current_animation_seconds = 0;;
    this->param.anime_index = static_cast<UINT>(animation_info.anime_index);
    this->param.anime_loop = animation_info.loop;
    this->param.anime_play = true;
}

void InstancedModelWithStateAnimationComponent::UpdateAnimationState()
{
#ifdef _DEBUG
    if (this->stop_anime_state_update) return;
#endif // DEBUG


    if (this->param.anime_index < 0) return;
    if (this->anime_state_pool.size() <= 0) return;

    for (auto& transition_info : this->anime_state_pool[this->param.anime_index].transition_info_pool)
    {
        if (PerformTransitionJudgement(transition_info->judgement.get()))
        {
            PlayAnimation(this->anime_state_pool[transition_info->next_anime_index]);
            break;
        }
    }
}

void InstancedModelWithStateAnimationComponent::SetAnimationState(AnimeIndex anime_index, bool loop, float transition_ready_time)
{
    auto& anime_state = this->anime_state_pool[anime_index];
    anime_state.loop = loop;
    anime_state.transition_ready_time = transition_ready_time;
}

void InstancedModelWithStateAnimationComponent::AddAnimationTransition(AnimeIndex anime_index, AnimeIndex transition_anime_index, std::unique_ptr<TransitionJudgementBase> judgement)
{
    auto& transition_info_pool = this->anime_state_pool[anime_index].transition_info_pool;
    // 遷移するアニメーションステート
    auto& transition_info = transition_info_pool.emplace_back();
    transition_info = std::make_unique<AnimeTransitionInfo>();
    transition_info->next_anime_index = transition_anime_index;
    transition_info->judgement = std::move(judgement);
}

bool InstancedModelWithStateAnimationComponent::IsTransitionReady()
{
    // 再生中でなければ準備完了
    if (!this->param.anime_play) return true;

    auto& anime_state = this->anime_state_pool[this->param.anime_index];

    if (anime_state.transition_ready_time >= 0 &&
        anime_state.transition_ready_time <= this->param.current_animation_seconds) return true;

    return false;
}

bool InstancedModelWithStateAnimationComponent::PerformTransitionJudgement(TransitionJudgementBase* judgemen)
{
    if (!judgemen) return false;
#ifdef _DEBUG
    if (!judgemen->GetIsActive()) return false;
#endif // _DEBUG
    auto owner = GetOwner();
    if (!owner) return false;

    // 遷移準備を待つフラグがオンの場合、遷移の準備が整うまで待機する
    if (judgemen->GetRequireTransitionReady() && !IsTransitionReady()) return false;

    return judgemen->GetShouldReversey() ? !judgemen->CheckTransitionCondition() : judgemen->CheckTransitionCondition();
}

#ifdef _DEBUG

void InstancedModelWithStateAnimationComponent::DrawDebugGUI()
{
    ImGui::Checkbox("Animation Play", &this->param.anime_play);

    DrawDebugAnimationGUI();

    char buffer[1024];
    ::strncpy_s(buffer, sizeof(buffer), model_filename, sizeof(buffer));
    ImGui::InputText("Model FileName", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue);
}

void InstancedModelWithStateAnimationComponent::DrawDebugAnimationGUI()
{
    int anime_index_int = static_cast<int>(this->param.anime_index);
    if (anime_index_int < 0) return;

    const auto& animation = this->model_resource->GetAnimations()[anime_index_int];

    std::string play_anime_name = this->animation_name_pool[anime_index_int];

    // 現在のフレーム数表示
    ImGui::SliderFloat("Current Animation Seconds", &this->param.current_animation_seconds, 0.0f, animation.seconds_length);
    int anime_frame_i = GetAnimeFrame();
    ImGui::InputInt("Anime Frame",&anime_frame_i);
    if (ImGui::ComboUI("Animation", play_anime_name, this->animation_name_pool, anime_index_int))
    {
        auto& anime_state = this->anime_state_pool[anime_index_int];
        PlayAnimation(anime_state);
    }
    ImGui::Checkbox("Animation Loop Flag", &this->param.anime_loop);

    if (this->is_draw_deletail) DrawDetail();
    else this->is_draw_deletail = ImGui::Button("Draw Animation Deletail");
}

void InstancedModelWithStateAnimationComponent::DrawDetail()
{
    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);

    ImGui::Begin("Animation Detail", &is_draw_deletail, ImGuiWindowFlags_None);

    std::string select_anime_name = this->animation_name_pool[this->select_animation_index];
    ImGui::ComboUI("Select Animation", select_anime_name, this->animation_name_pool, this->select_animation_index);

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
        float seconds_length = model_resource->GetAnimations()[this->select_animation_index].seconds_length;
        ImGui::InputFloat("Seconds Length", &seconds_length);
        int seconds_max_frame = static_cast<int>(instancing_model_resource->GetAnimationLengths()[this->select_animation_index]);
        ImGui::InputInt("Seconds Max Frame", &seconds_max_frame);
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