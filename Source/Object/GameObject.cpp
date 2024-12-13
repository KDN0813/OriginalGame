#include "GameObject.h"
#ifdef _DEBUG
#include "Debug/ImGuiHelper.h"
#endif // DEBUG

GameObject::GameObject()
    :Singleton(this)
{
}

void GameObject::Update()
{
    // 有効でないweak_ptrを除去する
    this->enemy_Wptr_pool.erase(
        std::remove_if(this->enemy_Wptr_pool.begin(), this->enemy_Wptr_pool.end(),
            [](const std::weak_ptr<Object>& weakPtr)
            {
                return weakPtr.expired();  // 無効なweak_ptrかどうかを確認
            }),
        this->enemy_Wptr_pool.end());
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
            if (std::shared_ptr<Object> stage = this->stage_foor_Wptr.lock())
            {
                object_name = stage->GetName();
            }
            ImGui::InputTextString("Object Name##GameObjectStageFoor", object_name);

            object_name = "Not Object";
            if (std::shared_ptr<Object> stage = this->stage_wall_Wptr.lock())
            {
                object_name = stage->GetName();
            }
            ImGui::InputTextString("Object Name##GameObjectStageWall", object_name);
        }

        if (ImGui::CollapsingHeader("Enemy##GameObject"))
        {
            size_t enemy_count = this->enemy_Wptr_pool.size();
            ImGui::InputSize_t("Enemy Count", enemy_count);

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