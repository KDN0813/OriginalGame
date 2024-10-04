#include <imgui.h>
#include <string>
#include "AnimeTransitionJudgement.h"
#include "Object/Object.h"
#include "Component/ModelComponent.h"

bool AnimeTransitionJudgementBase::PerformTransitionJudgement()
{
#ifdef _DEBUG
    if (!this->is_active) return false;
#endif // _DEBUG
    auto owner = this->owner_Wptr.lock();
    if (!owner) return true;
    auto model = owner->EnsureComponentValid<ModelComponent>(this->model_Wptr);
    if (!model) return true;

    // 遷移準備を待つフラグがオンの場合、遷移の準備が整うまで待機する
    if (this->require_transition_ready && !model->IsTransitionReady()) return false;
    
    return this->should_reverse ? !CheckTransitionCondition() : CheckTransitionCondition();
}

void AnimeTransitionJudgementBase::DrawCommonDebugGUI(int unique_id)
{
    std::string label = "Should Reverse ##" + std::to_string(unique_id);
    ImGui::Checkbox(label.c_str(), &this->should_reverse);
    ImGui::SameLine();
    label = "Require Transition Ready ##" + std::to_string(unique_id);
    ImGui::Checkbox(label.c_str(), &this->require_transition_ready);
}
