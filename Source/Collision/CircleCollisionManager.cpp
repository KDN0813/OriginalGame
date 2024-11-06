#include "CircleCollisionManager.h"
#include "Object/GameObject.h"
#include "Collision/CollisionParam.h"
#include "Collision/Collision.h"

#include "Component/CircleCollisionComponent.h"

void CircleCollisionManager::EvaluateCollision(const std::shared_ptr<CircleCollisionComponent>& collison)
{
	switch (collison->GetCollisionType())
	{
	case COLLISION_OBJECT_TYPE::PLAYER:
	{
		this->vs_enemy_collision_Wptr_pool.emplace_back(collison);
		break;
	}
	default:
		break;
	}
}

void CircleCollisionManager::VsEnemy()
{
	GameObject* game_object = GameObject::Instance();
	if (!game_object) return;

	// �G�l�~�[�ƐڐG������s���R���W�����̎擾
	for (const auto& collision_Wptr : this->vs_enemy_collision_Wptr_pool)
	{
		if (auto player_circle = collision_Wptr.lock())
		{
			// �G�擾
			for (const std::shared_ptr<Object>& enemy : game_object->GetEnemyPool())
			{
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
					player_circle->OnCollision(enemy_circle->GetOwner());
				}
			}
		}
	}

	// �v�f���N���A����
	this->vs_enemy_collision_Wptr_pool.clear();
}
