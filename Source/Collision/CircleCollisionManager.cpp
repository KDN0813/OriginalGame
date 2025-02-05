#include "CircleCollisionManager.h"
#include "Object/GameObject.h"
#include "Collision/CollisionParam.h"
#include "Collision/Collision.h"
#include "Object\Constant\PlayerConstant.h"

#include "Component/CircleCollisionComponent.h"

void CircleCollisionManager::EvaluateCollision(const std::shared_ptr<CircleCollisionComponent>& collison)
{
	switch (collison->GetCollisionType())
	{
	case COLLISION_OBJECT_TYPE::PLAYER_ATTACK:
	{
		this->vs_enemy_collision_Wptr_pool.emplace_back(collison);
		break;
	}
	case COLLISION_OBJECT_TYPE::ENEMY_ATTACK:
	{
		this->vs_player_collision_Wptr_pool.emplace_back(collison);
		break;
	}
	default:
		break;
	}
}

void CircleCollisionManager::VsEnemy()
{
	GameObject::Instance game_object = GameObject::GetInstance();
	if (!game_object.Get()) return;

	// エネミーと接触判定を行うコリジョンの取得
	for (const auto& collision_Wptr : this->vs_enemy_collision_Wptr_pool)
	{
		if (auto player_circle = collision_Wptr.lock())
		{
			// 敵の配列取得
			const auto& enemy_Wptr_pool = game_object->GetEnemyWptPool();

			// 敵取得
			for (const auto& enemy_Wptr : enemy_Wptr_pool)
			{
				auto enemy = enemy_Wptr.lock();
				if (!enemy) continue;
				if (!enemy->GetIsActive()) continue;
				const auto& enemy_circle = enemy->GetComponent<CircleCollisionComponent>();
				if (!enemy_circle) return;
				if (!enemy_circle->GetIsActive()) return;

				// プレイヤー(攻)Vs敵(受)の
				CircleHitResult player_hit_result{};
				CircleHitResult enemy_hit_result{};
				if (Collision::IntersectCircleVsCircle(
					player_circle->GetCircleParam(),
					enemy_circle->GetCircleParam(),
					player_hit_result,
					enemy_hit_result
				))
				{
					// ヒットリザルト設定
					player_circle->SetHitResult(player_hit_result);
					enemy_circle->SetHitResult(enemy_hit_result);

					// 接触処理
					player_circle->OnCollision(enemy);
					enemy_circle->OnCollision(player_circle->GetOwner());
				}
			}
		}
	}

	// 要素をクリアする
	this->vs_enemy_collision_Wptr_pool.clear();
}

void CircleCollisionManager::VsPlayer()
{
	GameObject::Instance game_object = GameObject::GetInstance();
	if (!game_object.Get()) return;

	// プレイヤーと接触判定を行うコリジョンの取得
	for (const auto& collision_Wptr : this->vs_player_collision_Wptr_pool)
	{
		if (auto enemy_circle = collision_Wptr.lock())
		{
			const auto& player = game_object->GetPlayer();
			if (!player) continue;
			if (!player->GetIsActive()) continue;
			const auto& player_defense_object = player->FindChildObject(PlayerConstant::DEFENSE_OBJECT_NAME);
			if (!player_defense_object) continue;
			if (!player_defense_object->GetIsActive()) continue;
			const auto& player_circle = player_defense_object->GetComponent<CircleCollisionComponent>();
			if (!player_circle) return;
			if (!player_circle->GetIsActive()) return;

			// 敵(攻)Vsプレイヤー(受)
			CircleHitResult enemy_hit_result{};
			CircleHitResult player_hit_result{};
			if (Collision::IntersectCircleVsCircle(
				enemy_circle->GetCircleParam(),
				player_circle->GetCircleParam(),
				enemy_hit_result,
				player_hit_result
			))
			{
				// ヒットリザルト設定
				enemy_circle->SetHitResult(enemy_hit_result);
				player_circle->SetHitResult(player_hit_result);

				// 接触処理
				//enemy_circle->OnCollision(player);
				player_circle->OnCollision(enemy_circle->GetOwner());
			}
		}
	}

	// 要素をクリアする
	this->vs_player_collision_Wptr_pool.clear();
}
