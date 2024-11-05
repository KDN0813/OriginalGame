#include "EnemyComponent.h"
#include "System/MyMath/MyMathf.h"
#include "System/MyMath/MYVECTOR3.h"
#include "Object/Object.h"
#ifdef _DEBUG
#include <imgui.h>
#include "Debug/DebugManager.h"
#endif // DEBUG


#include "Component/TransformComponent.h"
#include "Component/MovementComponent.h"
#include "Component/CircleCollisionComponent.h"
#include "Component/CharacterComponent.h"

void EnemyComponent::Start()
{
}

void EnemyComponent::End()
{
}

void EnemyComponent::Update(float elapsed_time)
{
	if (this->param.idle_timer > 0.0f)
	{
		this->param.idle_timer -= elapsed_time;
	}

	auto owner = GetOwner();
	if (!owner) return;

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

	// 死亡判定
	{
		const auto& character = owner->EnsureComponentValid<CharacterComponent>(this->character_Wptr);
		if (!character)return;

		if (!character->IsAlive())
		{
			// 死亡していたら自身を削除する
			owner->SetIsRemove(true);
		}
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
	ImGui::Checkbox("Move Validity Flag", &this->param.move_validity_flag);
}

void EnemyComponent::DrawDebugPrimitive()
{
	auto debug_render = DebugManager::Instance()->GetDebugPrimitiveRenderer();
	if (!this->IsAtTarget())
		debug_render->DrawSphere(this->param.target_position, this->param.radius, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
}

#endif // _DEBUG