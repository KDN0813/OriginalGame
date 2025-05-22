#include "GameObjectRegistry.h"
#ifdef _DEBUG
#include "Debug/ImGuiHelper.h"
#endif // DEBUG
#include "System\MyMath\MyMathf.h"
#include "Object\Constant\EnemyConstant.h"

#include "Component\TransformComponent.h"

GameObjectRegistry::GameObjectRegistry()
    :Singleton(this)
{
}

void GameObjectRegistry::Update()
{
    // �L���łȂ�weak_ptr����������
    this->enemy_Wptr_pool.erase(
        std::remove_if(this->enemy_Wptr_pool.begin(), this->enemy_Wptr_pool.end(),
            [](const std::weak_ptr<Object>& weakPtr)
            {
                return weakPtr.expired();  // ������weak_ptr���ǂ������m�F
            }),
        this->enemy_Wptr_pool.end());
}

void GameObjectRegistry::EnemyAllClear()
{
    for (auto& enemy_Wptr : this->enemy_Wptr_pool)
    {
        const auto& enemy = enemy_Wptr.lock();
        if (!enemy) continue;
        enemy->SetIsRemove(true);
    }
}

#ifdef _DEBUG

void GameObjectRegistry::DebugDrawGUI()
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
            if (std::shared_ptr<Object> stage = this->stage_floor_Wptr.lock())
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

            int enemy_id = 0;   // �G�l�~�[��ID(ImGui�ŕʃf�[�^�Ƃ��Ĉ����p)
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