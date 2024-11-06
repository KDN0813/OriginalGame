#include "GameObject.h"
#ifdef _DEBUG
#include "Debug/ImGuiHelper.h"
#endif // DEBUG

GameObject::GameObject()
    :Singleton(this)
{
}

#ifdef _DEBUG

void GameObject::DebugDrawGUI()
{
    if (ImGui::Begin("GameObject"))
    {
        if (ImGui::CollapsingHeader("Player##GameObject"))
        {
            std::string object_name = "Not Object";
            if (std::shared_ptr<Object> player = this->player_Wptr.lock())
            {
                object_name = player->GetName();
            }
            ImGui::InputTextString("Object Name##GameObjectPlayer", object_name);
        }

        if (ImGui::CollapsingHeader("Stage##GameObject"))
        {
            std::string object_name = "Not Object";
            if (std::shared_ptr<Object> stage = this->stage_Wptr.lock())
            {
                object_name = stage->GetName();
            }
            ImGui::InputTextString("Object Name##GameObjectStage", object_name);
        }

        if (ImGui::CollapsingHeader("Enemy##GameObject"))
        {
            int enemy_id = 0;   // エネミーのID(ImGuiで別データとして扱う用)
            for (const std::weak_ptr<Object>& enemy_Wptr : this->enemy_Wptr_pool)
            {
                if (std::shared_ptr<Object> enemy = enemy_Wptr.lock())
                {
                    std::string object_name = enemy->GetName();
                    std::string label = +"Object Name##GameObjectenemy" + std::to_string(enemy_id);
                    ImGui::InputTextString(label.c_str(), object_name);
                }
                ++enemy_id;
            }
        }
    }
    ImGui::End();
}

#endif // DEBUG