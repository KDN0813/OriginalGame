#include "EnemyComponent.h"
#include "System/MyMath/MyMathf.h"
#include "System/MyMath/MYVECTOR3.h"
#include "Object/Object.h"
#ifdef _DEBUG
#include <imgui.h>
#include "Debug/DebugManager.h"
#endif // DEBUG
#include "../Hlsl/ParticleDisp.h"
#include "Shader\ParticleSystem.h"
#include "Camera\CameraManager.h"
#include "System\GameData.h"
#include "Object\GameObjectRegistry.h"
#include "Object\Constant\EnemyConstant.h"

#include "Component/TransformComponent.h"
#include "Component/MovementComponent.h"
#include "Component/CircleCollisionComponent.h"
#include "Component/CharacterComponent.h"
#include "Component/InstancedModelWithAnimationComponent.h"
#include "Component\CameraComponent.h"
#include "Component\DamageComponent.h"
#include "Component\PlayerComponent.h"

void EnemyComponent::Start()
{
	auto owner = GetOwner();
	if (!owner) return;
	auto transform = owner->GetComponent<Transform3DComponent>(this->transform_Wptr);
	if (!transform) return;
}

void EnemyComponent::End()
{
}

void EnemyComponent::Update(float elapsed_time)
{
	const auto& owner = GetOwner();
	if (!owner) return;
	const auto& model_component = owner->GetComponent(this->model_Wptr);
	if (!model_component) return;
	const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);

	auto transform = owner->GetComponent<Transform3DComponent>(this->transform_Wptr);
	// 移動処理
	if (transform && this->param.move_validity_flag)
	{
		// 目的地点までのXZ平面での距離判定
		MYVECTOR3 Position = transform->GetWorldPosition();
		MYVECTOR3 Target_position = this->param.target_position;
		float distSq = (Target_position.GetMyVectorXZ() - Position.GetMyVectorXZ()).LengthSq();

		if (!this->IsAtTarget(distSq))
		{
			// 目的地点へ移動
			MoveToTarget(elapsed_time, transform, this->param.speed_rate);
		}
	}
}

void EnemyComponent::Move(float vx, float vz, float speed)
{
	const auto& owner = GetOwner();

	if (auto movement = owner->GetComponent<MovementComponent>(movement_Wptr))
	{
		movement->AddAccelerationXZ(vx * speed, vz * speed);
	}
}

void EnemyComponent::MoveToTarget(float elapsed_time, std::shared_ptr<Transform3DComponent>& transform, float speed_rate)
{
	// ターゲット方向への進行ベクトルを算出
	MYVECTOR3 Position = transform->GetWorldPosition();
	MYVECTOR3 Target_position = this->param.target_position;
	MYVECTOR3 Vec = (Target_position.GetMyVectorXZ() - Position.GetMyVectorXZ()).Normalize();
	float vx = Vec.GetX();
	float vz = Vec.GetZ();

	// 移動処理
	Move(vx, vz, this->param.move_speed * speed_rate);
}

void EnemyComponent::OnCollision(const std::shared_ptr<Object>& hit_object)
{
	if (this->param.pending_removal_flag) return;	// 削除待ちの場合Return

	const auto& owner = GetOwner();
	if (!owner) return;

	const auto& character = owner->GetComponent(this->character_Wptr);
	if (!character) return;
	if (character->IsInvincible()) return;	// 無敵状態ならReturn

	const auto collision = owner->GetComponent(circle_collision_Wptr);
	if (!collision) return;

	switch (collision->GetCircleHitResult().collision_role)
	{
	case COLLISION_ROLE::ATTACKER:	// 衝突を与えた時の処理
	{
		break;
	}
	case COLLISION_ROLE::RECEIVER:	// 衝突を受けた時の処理
	{
		// ダメージ処理
		{
			if (const auto& damage = hit_object->GetComponent<DamageComponent>())
			{
				character->ApplyDamage(damage->GetDamageAmount(), damage->GetInvincibleTime());
			
				if (!character->IsAlive())
				{
					// 死亡した場合
					{
						// 増加するスペシャル倍率に適応させる
						this->param.add_special_point *= damage->GetPointRate();

						// スペシャルポイント増加
						GameObjectRegistry::Instance game_object = GameObjectRegistry::GetInstance();
						if (const auto& player = game_object->GetPlayer())
						{
							if (const auto& player_component = player->GetComponent<PlayerComponent>())
							{
								player_component->AddSpecialPoint(GetAddSpecialPoint());
							}
						}

						// スコア加算
						GameData::Instance game_data = GameData::GetInstance();
						game_data->AddScore(1);
					}
				}
			}
		}
		// 斬撃Effect再生
		{
			MYVECTOR3 Pos{};    // 生成位置
			DirectX::XMFLOAT3 pos{};    // 生成位置

			// 発生位置計算
			{
				// トランスフォーム取得
				if (const auto& child_transform = owner->GetComponent(this->child_transform_Wptr))
				{
					// 生成位置を設定
					Pos = child_transform->GetWorldPosition();
				}

				// カメラの向取得
				MYVECTOR3 Forward = MYVECTOR3(1.0f, 0.0f, 0.0f);
				MYVECTOR3 Up = MYVECTOR3(0.0f, 1.0f, 0.0f);
				{
					CameraManager::Instance camera_manager = CameraManager::GetInstance();
					if (const auto& camera = camera_manager->GetCamera(CAMERA_TYPE::MAIN))
					{
						Forward = camera->GetForward();
						Up = camera->GetUp();
					}
				}

				// 再生位置計算
				Pos += (Up * this->param.damage_effect_offset_up);
				Pos.GetFlaot3(pos);
			}

			// エフェクト再生
			{
				ParticleSystem::Instance particle_system = ParticleSystem::GetInstance();
				particle_system->PlayEffect(
					EFFECT_HIT,
					pos,
					45.0f,
					DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f)
				);
			}
		}
	}
		break;
	default:
		break;
	}
}

void EnemyComponent::SetRandomTargetPosition()
{
	float theta = MyMath::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = MyMath::RandomRange(0.0f, this->param.territory_range);
	this->param.target_position.x = this->param.spawn_point.x + sinf(theta) * range;
	this->param.target_position.y = 0.0f;
	this->param.target_position.z = this->param.spawn_point.z + cosf(theta) * range;
}

void EnemyComponent::SetTargetPositionByPlayer()
{
	{
		GameObjectRegistry::Instance game_object = GameObjectRegistry::GetInstance();
		const auto& player = game_object->GetPlayer();
		if (!player) return;
		const auto& player_transform = player->GetComponent<Transform3DComponent>(this->player_transform_Wptr);
		if (!player_transform) return;
		this->param.target_position = player_transform->GetWorldPosition();
	}
}

void EnemyComponent::SetRandomIdleTime()
{
	this->param.idle_timer = MyMath::RandomRange(this->param.min_idle_time, this->param.max_idle_time);
}

bool EnemyComponent::IsAtTarget()
{
	auto owner = GetOwner();
	if (!owner) return false;
	auto transform = owner->GetComponent<Transform3DComponent>(this->transform_Wptr);
	if (!transform) return false;

	// 目的地点までのXZ平面での距離判定
	MYVECTOR3 Position = transform->GetWorldPosition();
	MYVECTOR3 Target_position = this->param.target_position;
	float distSq = (Target_position.GetMyVectorXZ() - Position.GetMyVectorXZ()).LengthSq();

	return IsAtTarget(distSq);
}

bool EnemyComponent::IsAtTarget(float distSq)
{
	return (distSq < this->param.radius * this->param.radius);
}

bool EnemyComponent::IsPlayerInMovementArea()
{
	MYVECTOR3 SpawnPosition = this->param.spawn_point;	// 生成位置(移動範囲の中心)
	MYVECTOR3 Player_position{};					// プレイヤーの位置

	// プレイヤー取得
	GameObjectRegistry::Instance game_object = GameObjectRegistry::GetInstance();
	const auto& player = game_object->GetPlayer();
	if (!player) return false;

	// プレイヤーの位置設定
	const auto& player_transform = player->GetComponent<Transform3DComponent>(this->player_transform_Wptr);
	if (!player_transform) return false;
	Player_position = player_transform->GetWorldPosition();
	
	// 範囲内に存在するか判定する
	float distSq = (Player_position.GetMyVectorXZ() - SpawnPosition.GetMyVectorXZ()).LengthSq();
	return (distSq < this->param.territory_range * this->param.territory_range);
}

bool EnemyComponent::IsPlayerInAttackArea()
{
	// 自身の位置取得
	auto owner = GetOwner();
	if (!owner) return false;
	auto transform = owner->GetComponent<Transform3DComponent>(this->transform_Wptr);
	if (!transform) return false;
	MYVECTOR3 Position = transform->GetWorldPosition();

	// プレイヤー取得
	GameObjectRegistry::Instance game_object = GameObjectRegistry::GetInstance();
	const auto& player = game_object->GetPlayer();
	if (!player) return false;
	MYVECTOR3 Player_position{};

	// プレイヤーの位置設定
	const auto& player_transform = player->GetComponent<Transform3DComponent>(this->player_transform_Wptr);
	if (!player_transform) return false;
	Player_position = player_transform->GetWorldPosition();

	// 範囲内に存在するか判定する
	float distSq = (Player_position.GetMyVectorXZ() - Position.GetMyVectorXZ()).LengthSq();
	return (distSq < this->param.attack_range * this->param.attack_range);
}

void EnemyComponent::SetDamageState()
{
	const auto& owner = GetOwner();
	if (!owner) return;
	const auto& model_component = owner->GetComponent(this->model_Wptr);
	if (!model_component) return;

	this->param.state = STATE::DAMAGE;
	model_component->PlayAnimation(EnemyConstant::ANIMATION::TAUNTING, false);
}

#ifdef _DEBUG

void EnemyComponent::DrawDebugGUI()
{
	ImGui::InputFloat3("Target Position", &this->param.target_position.x);
	ImGui::DragFloat("Territory Range", &this->param.territory_range, 0.01f);
	ImGui::DragFloat("Radius", &this->param.radius, 0.01f);
	ImGui::DragFloat("Move Speed", &this->param.move_speed);
	ImGui::DragFloat("Speed Rate", &this->param.speed_rate);
	ImGui::InputFloat("Idle Timer", &this->param.idle_timer);
	ImGui::InputFloat("Idle Max Timer", &this->param.max_idle_time);
	ImGui::InputFloat("Idle Min Timer", &this->param.min_idle_time);
	ImGui::InputFloat("Remove Timer", &this->param.remove_timer);
	ImGui::InputFloat("Damage Effect Offset Up", &this->param.damage_effect_offset_up);
	ImGui::InputFloat("Add Special Point", &this->param.add_special_point);
	ImGui::Checkbox("Move Validity Flag", &this->param.move_validity_flag);
	ImGui::Checkbox("Pending Removal Flag", &this->param.pending_removal_flag);
}

void EnemyComponent::DrawDebugPrimitive()
{
	DebugManager::Instance debug_manager = DebugManager::GetInstance();
	auto debug_render = debug_manager->GetDebugPrimitiveRenderer();

	const auto& owner = GetOwner();
	if (!owner) return;
	auto transform = owner->GetComponent<Transform3DComponent>(this->transform_Wptr);
	if (!transform) return;
	MYVECTOR3 Position = transform->GetWorldPosition();
	
	if (debug_render)
	{		
		if (IsPlayerInMovementArea())
		{
			// 攻撃可能範囲
			debug_render->DrawCylinder(transform->GetWorldPosition(), this->param.attack_range, 2.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		}
	}
}

#endif // _DEBUG