#include "EnemyComponent.h"
#include "System/MyMath/MyMathf.h"
#include "System/MyMath/MYVECTOR3.h"
#include "Object/Object.h"
#ifdef _DEBUG
#include <imgui.h>
#include "Debug/DebugManager.h"
#endif // DEBUG
#include "ConstantManager.h"
#include "Shader\ParticleSystem.h"

#include "Component/TransformComponent.h"
#include "Component/MovementComponent.h"
#include "Component/CircleCollisionComponent.h"
#include "Component/CharacterComponent.h"
#include "Component/InstancedModelWithAnimationComponent.h"

void EnemyComponent::Start()
{
}

void EnemyComponent::End()
{
}

void EnemyComponent::Update(float elapsed_time)
{
	const auto& owner = GetOwner();
	if (!owner) return;
	const auto& model_component = owner->EnsureComponentValid(this->model_Wptr);
	if (!model_component) return;
	const auto& character = owner->EnsureComponentValid<CharacterComponent>(this->character_Wptr);

	// 死亡判定
	if (character && !character->IsAlive() && !this->param.pending_removal_flag)
	{
		// 削除待ちフラグを立てる
		this->param.pending_removal_flag = true;

		// 死亡状態へ遷移
		this->param.state = STATE::DETH;

		// 死亡状態への準備
		{
			model_component->PlayAnimation(EnemyCT::ANIMATION::DIE, false);
		}
	}

	// ステート更新
	switch (this->param.state)
	{
	case EnemyComponent::STATE::IDLE:
	{
		if (this->param.idle_timer > 0.0f)
		{
			// 待機タイマー更新
			this->param.idle_timer -= elapsed_time;
		}
		else
		{
			// 待機時間が終了したら移動状態に遷移
			this->param.state = STATE::MOVE;

			// 移動状態への準備
			{
				model_component->PlayAnimation(EnemyCT::MOVE_FWD, true);
				SetRandomTargetPosition();
				return;
			}
		}
		break;
	}
	case EnemyComponent::STATE::MOVE:
	{
		// 移動処理
		{
			auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr);
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

		if (IsAtTarget())
		{
			// 目的地に到着したら待機状態に遷移
			this->param.state = STATE::IDLE;

			// 待機状態への準備
			{
				model_component->PlayAnimation(EnemyCT::IDLE_BATTLE, true);
				SetRandomIdleTime();
				return;
			}
		}

		break;
	}
	case EnemyComponent::STATE::DAMAGE:
	{
		if (!model_component->IsPlayAnime())
		{
			// アニメーションが終了移動状態に遷移
			this->param.state = STATE::MOVE;

			// 移動状態への準備
			{
				model_component->PlayAnimation(EnemyCT::MOVE_FWD, true);
				SetRandomTargetPosition();
				return;
			}
		}

		break;
	}
	case EnemyComponent::STATE::DETH:
	{
		if (model_component->IsPlayAnime()) break;	// アニメーション再生中ならbreak

		if (this->param.remove_timer > 0.0f)
		{
			// 削除タイマー更新
			this->param.remove_timer -= elapsed_time;
		}
		else
		{
			owner->SetIsRemove(true);
		}
		break;
	}
	default:break;
	}
}

void EnemyComponent::Move(float vx, float vz, float speed)
{
	auto owner = GetOwner();

	if (auto movement = owner->EnsureComponentValid<MovementComponent>(movement_Wptr))
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
	// ダメージステートに遷移させる
	SetDamageState();

	// 斬撃effect再生
	{
		DirectX::XMFLOAT3 pos{};    // 生成位置
		
		// 発生位置取得
		if (const auto& owner = GetOwner())
		{
			// エフェクトの再生位置を管理する子オブジェクトの取得
			if (const auto& child_object = owner->FindChildObject(MyHash("SlashEffectObject")))
			{
				// トランスフォーム取得
				if (const auto& child_transform = child_object->EnsureComponentValid(this->child_transform_Wptr))
				{
					// 生成位置を設定
					pos = child_transform->GetWorldPosition();
				}
			}
		}

		// エフェクト再生
		if (ParticleSystem::Instance particle_system = ParticleSystem::GetInstance(); particle_system.Get())
		{
			particle_system->Set(
				pos,
				45.0f
			);
		}
	}
}

void EnemyComponent::SetRandomTargetPosition()
{
	float theta = MyMathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = MyMathf::RandomRange(0.0f, this->param.territory_range);
	this->param.target_position.x =  + sinf(theta) * range;
	this->param.target_position.y = 0.0f;
	this->param.target_position.z =  + cosf(theta) * range;
}

void EnemyComponent::SetRandomIdleTime()
{
	this->param.idle_timer = MyMathf::RandomRange(this->param.min_idle_time, this->param.max_idle_time);
}

bool EnemyComponent::IsAtTarget()
{
	auto owner = GetOwner();
	if (!owner) return false;
	auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr);
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

void EnemyComponent::SetDamageState()
{
	if (this->param.pending_removal_flag) return;	// 削除待ちの場合return

	const auto& owner = GetOwner();
	if (!owner) return;
	const auto& model_component = owner->EnsureComponentValid(this->model_Wptr);
	if (!model_component) return;

	this->param.state = STATE::DAMAGE;
	model_component->PlayAnimation(EnemyCT::ANIMATION::TAUNTING, false);
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
	ImGui::Checkbox("Move Validity Flag", &this->param.move_validity_flag);
	ImGui::Checkbox("Pending Removal Flag", &this->param.pending_removal_flag);
}

void EnemyComponent::DrawDebugPrimitive()
{
	DebugManager::Instance debug_manager = DebugManager::GetInstance();
	if (!debug_manager.Get()) return;
	auto debug_render = debug_manager->GetDebugPrimitiveRenderer();
	if (debug_render && !this->IsAtTarget())
		debug_render->DrawSphere(this->param.target_position, this->param.radius, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
}

#endif // _DEBUG