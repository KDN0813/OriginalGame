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

void EnemyComponent::Start()
{
}

void EnemyComponent::End()
{
}

void EnemyComponent::Update(float elapsed_time)
{
	if (idle_timer > 0.0f)
	{
		idle_timer -= elapsed_time;
	}

	auto owner = GetOwner();
	if (!owner) return;
	auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr);
	if (transform)
	{
		// 目的地点までのXZ平面での距離判定
		MYVECTOR3 Position = transform->GetWorldPosition();
		MYVECTOR3 Target_position = this->target_position;
		float distSq = (Target_position.GetMyVectorXZ() - Position.GetMyVectorXZ()).LengthSq();

		if (!this->IsAtTarget(distSq))
		{
			// 目的地点へ移動
			MoveToTarget(elapsed_time, transform, speed_rate);
		}
	}
	auto circle_collision = owner->EnsureComponentValid<CircleCollisionComponent>(this->circle_collision_Wptr);
	if (circle_collision)
	{
		if (circle_collision->GetHitFlag())
		{
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
	MYVECTOR3 Target_position = this->target_position;
	MYVECTOR3 Vec = (Target_position.GetMyVectorXZ() - Position.GetMyVectorXZ()).Normalize();
	float vx = Vec.GetX();
	float vz = Vec.GetZ();

	// 移動処理
	Move(vx, vz, move_speed * speed_rate);
}

void EnemyComponent::SetRandomTargetPosition()
{
	float theta = MyMathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = MyMathf::RandomRange(0.0f, this->territory_range);
	this->target_position.x =  + sinf(theta) * range;
	this->target_position.y = 0.0f;
	this->target_position.z =  + cosf(theta) * range;
}

void EnemyComponent::SetRandomIdleTime()
{
	this->idle_timer = MyMathf::RandomRange(this->min_idle_time, this->max_idle_time);
}

bool EnemyComponent::IsAtTarget()
{
	auto owner = GetOwner();
	if (!owner) return false;
	auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr);
	if (!transform) return false;

	// 目的地点までのXZ平面での距離判定
	MYVECTOR3 Position = transform->GetWorldPosition();
	MYVECTOR3 Target_position = this->target_position;
	float distSq = (Target_position.GetMyVectorXZ() - Position.GetMyVectorXZ()).LengthSq();

	return IsAtTarget(distSq);
}

bool EnemyComponent::IsAtTarget(float distSq)
{
	return (distSq < this->radius * this->radius);
}

#ifdef _DEBUG

void EnemyComponent::DrawDebugGUI()
{
	ImGui::InputFloat("Idle State", &this->idle_timer);
	ImGui::InputFloat3("Target Position", &this->target_position.x);
	ImGui::DragFloat("Territory Range", &this->territory_range, 0.01f);
	ImGui::DragFloat("Radius", &this->radius, 0.01f);
	ImGui::DragFloat("Move Speed", &this->move_speed);
	ImGui::DragFloat("Speed Rate", &this->speed_rate);
}

void EnemyComponent::DrawDebugPrimitive()
{
	auto debug_render = DebugManager::Instance()->GetDebugPrimitiveRenderer();
	if (!this->IsAtTarget())
		debug_render->DrawSphere(this->target_position, this->radius, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
}

#endif // _DEBUG