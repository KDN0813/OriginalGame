#include "EnemyComponent.h"
#include "System/MyMath/MyMathf.h"
#include "System/MyMath/MYVECTOR3.h"
#include "Object/Object.h"

#include "Component/TransformComponent.h"
#include "Component/MovementComponent.h"

void EnemyComponent::Start()
{
	SetIsActive(false);
}

void EnemyComponent::End()
{
}

void EnemyComponent::Update(float elapsed_time)
{
	auto owner = GetOwner();
	if (!owner) return;
	auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr);
	if (!transform) return;

	// 目的地点までのXZ平面での距離判定
	MYVECTOR3 Position = transform->GetPosition();
	MYVECTOR3 Target_position = this->target_position;
	float distSq = (Target_position.GetMyVectorXZ() - Position.GetMyVectorXZ()).LengthSq();
	
	// 目的地点へ移動
	MoveToTarget(elapsed_time, transform, speed_rate);
	
	// 目的地へ着いた
	if (distSq < radius * radius)
	{
		SetRandomTargetPosition();
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
	MYVECTOR3 Position = transform->GetPosition();
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
	float range = MyMathf::RandomRange(0.0f, territory_range);
	this->target_position.x =  + sinf(theta) * range;
	this->target_position.y = 0.0f;
	this->target_position.z =  + cosf(theta) * range;
}

#ifdef _DEBUG

void EnemyComponent::DrawDebugGUI()
{
}

#endif // _DEBUG