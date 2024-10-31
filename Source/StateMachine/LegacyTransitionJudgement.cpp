#include <imgui.h>
#include <string>
#include "LegacyTransitionJudgement.h"
#include "Object/Object.h"
#include "Component/ModelComponent.h"

#ifdef _DEBUG

void TransitionJudgementBase::DrawCommonDebugGUI(int unique_id)
{
    std::string label = "Should Reverse ##" + std::to_string(unique_id);
    ImGui::Checkbox(label.c_str(), &this->should_reverse);
    ImGui::SameLine();
    label = "Require Transition Ready ##" + std::to_string(unique_id);
    ImGui::Checkbox(label.c_str(), &this->require_transition_ready);
}

#endif // _DEBUG