#include "EnemySpawnerComponent.h"

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
}

void EnemySpawnerComponent::Update(float elapsed_time)
{
    if (!this->param.is_create_enemy) return;

    // 敵の生成
    this->param.create_cool_timer -= elapsed_time;
    if (this->param.create_cool_timer <= 0.0f)
    {
        AddCreateEnemy(this->object_manager_Wptr.lock());

        const float COOL_TIME = MyMath::RandomRange(this->param.create_cool_time_min, this->param.create_cool_time_max);
        this->param.create_cool_timer = COOL_TIME;
    }
}

void EnemySpawnerComponent::AddCreateEnemy(const std::shared_ptr<ObjectManager>& manager)
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

        // 敵の生成
        const auto& enemy = EnemyConstant::CreateEnemy(spawn_point, manager->Create());
        // 敵をGameObjectRegistryに設定
        object_registry->SetEnemy(enemy);
    }

}
