#include "EnemySpawnerComponent.h"

#ifdef _DEBUG
#include <imgui.h>
#include "Debug\DebugManager.h"
#endif // _DEBUG
#include "Object\Object.h"
#include "Object\GameObjectRegistry.h"

#include "System\MyMath\MyMathf.h"
#include "Object\Object.h"
#include "Object\Constant\EnemyConstant.h"

#include "Component\TransformComponent.h"

EnemySpawnerComponent::EnemySpawnerComponent(Param param)
    : param(param)
    , default_param(param)
{
    this->enemy_config_pool.resize(EyemyType::Max);
    auto& enemy_type1 = this->enemy_config_pool.at(EyemyType::Normal);
    enemy_type1.spawn_point = {};
    enemy_type1.base_color = { 1.0f,1.0f, 1.0f, 1.0f };
    enemy_type1.move_speed = 5.0f;
}

void EnemySpawnerComponent::Start()
{
#ifdef _DEBUG
    this->min_spawn_area.SetRadius(this->param.min_spawn_dist);
    this->min_spawn_area.SetHeight(4.0f);
    this->min_spawn_area.SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f));
    this->max_spawn_area.SetRadius(this->param.max_spawn_dist);
    this->max_spawn_area.SetHeight(4.0f);
    this->max_spawn_area.SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f));
#endif // _DEBUG
}

void EnemySpawnerComponent::Update(float elapsed_time)
{
    // 敵の生成
    this->param.create_cool_timer -= elapsed_time;
    if (this->param.create_cool_timer <= 0.0f)
    {
        UpdateEnemySpawner(this->object_manager_Wptr.lock());

        const float COOL_TIME = MyMath::RandomRange(this->param.create_cool_time_min, this->param.create_cool_time_max);
        this->param.create_cool_timer = COOL_TIME;
    }

#ifdef _DEBUG   // 生成範囲を示すプリミティブの位置を更新
    GameObjectRegistry::Instance object_registry = GameObjectRegistry::GetInstance();

    const auto& player = object_registry->GetPlayer();
    if (!player) return;
    const auto& player_transform = player->GetComponent<Transform3DComponent>(this->player_transform_Wptr);
    if (!player_transform) return;

    this->min_spawn_area.SetPosition(player_transform->GetWorldPosition());
    this->max_spawn_area.SetPosition(player_transform->GetWorldPosition());
#endif // _DEBUG
}

void EnemySpawnerComponent::UpdateEnemySpawner(const std::shared_ptr<ObjectManager>& manager)
{
    if (manager == nullptr) return;

    GameObjectRegistry::Instance object_registry = GameObjectRegistry::GetInstance();

    const auto& player = object_registry->GetPlayer();
    if (!player) return;
    const auto& player_transform = player->GetComponent<Transform3DComponent>(this->player_transform_Wptr);
    if (!player_transform) return;
    const DirectX::XMFLOAT3 PLAYER_POS = player_transform->GetWorldPosition();

    const std::vector<std::weak_ptr<Object>>& enemy_Wptr_pool = object_registry->GetEnemyWptPool();

    const size_t NOW_ENEMY_COUNT = enemy_Wptr_pool.size();  // 現在の敵の数
    const size_t ENEMY_MAX = EnemyConstant::ENEMY_MAX;      // 敵の最大値
    const size_t CREATE_ENEMY_MAX = MyMath::RandomRange(this->param.min_enemies_per_spawn, this->param.max_enemies_per_spawn);	// 1度に生成するエネミーの数

    for (int i = 0; i < ENEMY_MAX - NOW_ENEMY_COUNT; ++i)
    {
        if (CREATE_ENEMY_MAX <= i)break;

        const DirectX::XMFLOAT3 SPAWN_POINT = MyMath::GetNonOverlappingPointInRing(PLAYER_POS, this->param.min_spawn_dist, this->param.max_spawn_dist);

        EnemyConfig config = this->enemy_config_pool.at(EyemyType::Normal);
        config.spawn_point = SPAWN_POINT;
        CreateEnemy(manager, config);
    }

}

void EnemySpawnerComponent::CreateEnemy(const std::shared_ptr<ObjectManager>& manager, const EnemyConfig& config)
{
    GameObjectRegistry::Instance object_registry = GameObjectRegistry::GetInstance();
    // 敵の生成
    const auto& enemy = EnemyConstant::CreateEnemy(manager->Create(), chain_score_counter_object_Wptr.lock(), config);
    // 敵をGameObjectRegistryに設定
    object_registry->SetEnemy(enemy);
}

#ifdef _DEBUG

void EnemySpawnerComponent::DrawDebugGUI()
{
    if (ImGui::InputFloat("Create Cool Time Max", &this->param.create_cool_time_max))
    {
        this->param.create_cool_time_max = (std::min)(this->param.create_cool_time_max, 0.0f);
    }
    if (ImGui::InputFloat("Create Cool Time Min", &this->param.create_cool_time_min))
    {
        this->param.create_cool_time_min = (std::min)(this->param.create_cool_time_min, 0.0f);
    }
    ImGui::SliderFloat("Cool Time", &this->param.create_cool_timer, 0.0f, this->param.create_cool_time_max);

    ImGui::InputInt("Min Enemies Per Spawn", &this->param.min_enemies_per_spawn);
    ImGui::InputInt("Max Enemies Per Spawn", &this->param.max_enemies_per_spawn);

    if (ImGui::Button("Create Enemy"))
    {
        EnemyConfig config = this->enemy_config_pool.at(EyemyType::Normal);
        CreateEnemy(this->object_manager_Wptr.lock(), config);
    }

    if (ImGui::InputFloat("Min Spawn Dist", &this->param.min_spawn_dist))
    {
        this->min_spawn_area.SetRadius(this->param.min_spawn_dist);
    }
    if (ImGui::InputFloat("Max Spawn Dist", &this->param.max_spawn_dist))
    {
        this->max_spawn_area.SetRadius(this->param.max_spawn_dist);
    }
}

void EnemySpawnerComponent::DrawDebugPrimitive()
{
    const auto& debug_primitive_render = DebugManager::GetInstance()->GetDebugPrimitiveRenderer();
    debug_primitive_render->DrawCylinder(this->min_spawn_area);
    debug_primitive_render->DrawCylinder(this->max_spawn_area);
}

void EnemySpawnerComponent::DrawDebugPrimitiveGUI()
{
    this->min_spawn_area.DrawDebugGUI("Min Spawn Area");
    this->max_spawn_area.DrawDebugGUI("Max Spawn Area");
}

#endif // _DEBUG