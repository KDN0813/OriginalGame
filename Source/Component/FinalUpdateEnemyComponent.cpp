#include "FinalUpdateEnemyComponent.h"

#include <cmath>
#include "System\MyMath\MYVECTOR3.h"
#include "Object\GameObjectRegistry.h"
#include "Object\Constant\StageConstant.h"
#include "GridObjectManager\GridObjectManager.h"

#include "Component\EnemyComponent.h"
#include "Component\TransformComponent.h"
#include "Component\CharacterComponent.h"

void FinalUpdateEnemyComponent::Start()
{
    // 自身の半径取得
    const auto& owner = GetOwner();
    if (!owner) return;
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    const float radius = character->GetRadius();

    // プレイヤーの半径取得
    GameObjectRegistry::Instance game_object = GameObjectRegistry::GetInstance();
    const auto& player = game_object->GetPlayer();
    if (!player) return;
    const auto& player_character = player->GetComponent<CharacterComponent>(this->player_character_Wptr);
    if (!player_character) return;
    const float player_radius = player_character->GetRadius();

    // プレイヤーとの当たり判定を行う範囲を計算
    this->collision_range = static_cast<int>(std::ceil(static_cast<double>(player_radius + radius) / GridObjectManager::CELL_SIZE));
}

void FinalUpdateEnemyComponent::Update(float elapsed_time)
{
    const auto& owner = GetOwner();
    if (!owner) return;
    const auto& transform = owner->GetComponent<Transform3DComponent>(this->transform_Wptr);
    if (!transform) return;
    // 自身の半径取得
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    const float radius = character->GetRadius();

    // プレイヤー取得
    GameObjectRegistry::Instance game_object = GameObjectRegistry::GetInstance();
    const auto& player = game_object->GetPlayer();
    if (!player) return;
    // プレイヤーのトランスフォーム取得
    const auto& player_transform = player->GetComponent<Transform3DComponent>(this->player_transform_Wptr);
    if (!player_transform) return;

    // 自身のセル検索
    DirectX::XMFLOAT3 pos = transform->GetWorldPosition();
    GridObjectManager::Instance grid_object_maanager = GridObjectManager::GetInstance();
    // プレイヤーのセル検索
    DirectX::XMFLOAT3 player_pos = player_transform->GetWorldPosition();
    // グリッド間の距離
    const int grid_spacing = grid_object_maanager->GetDistanceInGrid(pos, player_pos);

    // プレイヤーの一定範囲内に自身が存在すれば
    if (grid_spacing <= this->collision_range)
    {
        // 衝突判定を行う
        MYVECTOR3 Pos = pos;
        MYVECTOR3 Player_pos = player_pos;
        MYVECTOR3 Vec = Pos - Player_pos;

        const auto& player_character = player->GetComponent<CharacterComponent>(this->player_character_Wptr);
        if (!player_character) return;
        const float player_radius = player_character->GetRadius();

        const float  push_radius = radius + player_radius;

        const float vec_range = Vec.LengthSq();
        if (vec_range < push_radius * push_radius)
        {
            // 衝突していたら、押し出し処理を行う
            Vec.NormalizeSelf();
            MYVECTOR3 New_pos = Player_pos + Vec * push_radius;
            DirectX::XMFLOAT3 new_pos{};
            New_pos.GetFlaot3(new_pos);
            new_pos.y = StageConstant::STAGE_FLOOR_Y;   // Y座標は床の位置で固定する

            transform->SetLocalPosition(new_pos);
        }

#ifdef _DEBUG
        character->SetIsDebugPrimitive(true);   // 一定距離内にいる時だけ半径を描画する
#endif // _DEBUG
    }
#ifdef _DEBUG
    else
    {
        character->SetIsDebugPrimitive(false);
    }

#endif // _DEBUG
}
