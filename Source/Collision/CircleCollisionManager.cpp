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

	// �G�l�~�[�ƐڐG������s���R���W�����̎擾
	for (const auto& collision_Wptr : this->vs_enemy_collision_Wptr_pool)
	{
		if (auto player_circle = collision_Wptr.lock())
		{
			// �G�̔z��擾
			const auto& enemy_Wptr_pool = game_object->GetEnemyWptPool();

			// �G�擾
			for (const auto& enemy_Wptr : enemy_Wptr_pool)
			{
				auto enemy = enemy_Wptr.lock();
				if (!enemy) continue;
				if (!enemy->GetIsActive()) continue;
				const auto& enemy_circle = enemy->GetComponent<CircleCollisionComponent>();
				if (!enemy_circle) return;
				if (!enemy_circle->GetIsActive()) return;

				// �v���C���[(�U)Vs�G(��)��
				CircleHitResult player_hit_result{};
				CircleHitResult enemy_hit_result{};
				if (Collision::IntersectCircleVsCircle(
					player_circle->GetCircleParam(),
					enemy_circle->GetCircleParam(),
					player_hit_result,
					enemy_hit_result
				))
				{
					// �q�b�g���U���g�ݒ�
					player_circle->SetHitResult(player_hit_result);
					enemy_circle->SetHitResult(enemy_hit_result);

					// �ڐG����
					player_circle->OnCollision(enemy);
					enemy_circle->OnCollision(player_circle->GetOwner());
				}
			}
		}
	}

	// �v�f���N���A����
	this->vs_enemy_collision_Wptr_pool.clear();
}

void CircleCollisionManager::VsPlayer()
{
	GameObject::Instance game_object = GameObject::GetInstance();
	if (!game_object.Get()) return;

	// �v���C���[�ƐڐG������s���R���W�����̎擾
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

			// �G(�U)Vs�v���C���[(��)
			CircleHitResult enemy_hit_result{};
			CircleHitResult player_hit_result{};
			if (Collision::IntersectCircleVsCircle(
				enemy_circle->GetCircleParam(),
				player_circle->GetCircleParam(),
				enemy_hit_result,
				player_hit_result
			))
			{
				// �q�b�g���U���g�ݒ�
				enemy_circle->SetHitResult(enemy_hit_result);
				player_circle->SetHitResult(player_hit_result);

				// �ڐG����
				//enemy_circle->OnCollision(player);
				player_circle->OnCollision(enemy_circle->GetOwner());
			}
		}
	}

	// �v�f���N���A����
	this->vs_player_collision_Wptr_pool.clear();
}
