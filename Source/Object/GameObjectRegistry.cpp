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
    // 有効でないweak_ptrを除去する
    this->enemy_Wptr_pool.erase(
        std::remove_if(this->enemy_Wptr_pool.begin(), this->enemy_Wptr_pool.end(),
            [](const std::weak_ptr<Object>& weakPtr)
            {
                return weakPtr.expired();  // 無効なweak_ptrかどうかを確認
            }),
        this->enemy_Wptr_pool.end());
}

void GameObjectRegistry::AddCreateEnemy(float elapsed_time, ObjectManager& object_manager)
{
    if (!this->is_create_enemy) return;

    // 敵の生成
    this->create_enemy_cool_time -= elapsed_time;
    if (create_enemy_cool_time <= 0.0f)
    {
        AddCreateEnemy(object_manager);

        const float COOL_TIME = MyMath::RandomRange(3.0f, 5.0f);
        create_enemy_cool_time = COOL_TIME;
    }
}

void GameObjectRegistry::AddCreateEnemy(ObjectManager& object_manager)
{
    const auto& player = GetPlayer();
    if (!player) return;
    const auto& player_transform = player->GetComponent<Transform3DComponent>(this->player_transform_Wptr);
    if (!player_transform) return;
    const DirectX::XMFLOAT3 PLAYER_POS = player_transform->GetWorldPosition();

    const size_t NOW_ENEMY_COUNT = this->GetEnemyWptPool().size();
    const size_t ENEMY_MAX = EnemyConstant::ENEMY_MAX;
#ifdef _DEBUG
    const size_t CREATE_ENEMY_MAX = MyMath::RandomRange(10, 20);	// 1度に生成できるエネミーの最大数
#else
    const size_t CREATE_ENEMY_MAX = MyMath::RandomRange(80, 150);	// 1度に生成できるエネミーの最大数
#endif // _DEBUG

    for (int i = 0; i < ENEMY_MAX - NOW_ENEMY_COUNT; ++i)
    {
        if (CREATE_ENEMY_MAX <= i)break;

        const float player_area_rage = 20.0f;
        const DirectX::XMFLOAT3 spawn_point = MyMath::GetNonOverlappingPointInRing(PLAYER_POS, player_area_rage, EnemyConstant::NEAR_PLAYER_TERRITORY_RENGR);

        const auto& enemy = EnemyConstant::CreateEnemy(spawn_point, object_manager.Create());
        SetEnemy(enemy);
    }
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
            ImGui::Checkbox("Is Create Enemy", &this->is_create_enemy);

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