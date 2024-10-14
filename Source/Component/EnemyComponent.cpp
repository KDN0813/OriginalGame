#include "EnemyComponent.h"
#include "System/MyMath/MyMathf.h"
#include "System/MyMath/MYVECTOR3.h"
#include "Object/Object.h"

#include "Component/TransformComponent.h"
#include "Component/MovementComponent.h"

void EnemyComponent::Start()
{
	SetRandomTargetPosition();
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

void EnemyComponent::Turn(float elapsed_time, float vx, float vz, float speed)
{
	// 進行ベクトルが0以下なら処理しない
	float length = sqrtf(vx * vx + vz * vz);
	if (length < 0.001) return;

	auto owner = GetOwner();
	if (auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr))
	{
		speed *= elapsed_time;

		// 単位ベクトル化
		vx /= length;
		vz /= length;

		// 自身の回転値から前方向を求める
		DirectX::XMFLOAT3 angle = transform->GetAngle();
		float frontX = sinf(angle.y);
		float frontZ = cosf(angle.y);

		// 回転角を求めるため、2つの単位ベクトルの内積を計算する
		float dot = (frontX * vx + frontZ * vz);

		// 内積値は-1.0~1.0で表現されており、2つの単位ベクトルの角度が
		// 小さいほど、1.0に近づくという性質を利用して回転速度を調整する
		float rot = 1.0f - dot;
		if (rot > speed) rot = speed;

		// 左右判定を行うために2つの単位ベクトルの外積を計算する
		float cross = (frontZ * vx) - (frontX * vz);

		// 2Dの外積値が正の場合か負の場合によって左右判定が行える
		//  左右判定を行うことによって左右回転を選択する
		if (cross < 0.0f)
		{
			angle.y -= rot;
		}
		else
		{
			angle.y += rot;
		}

		transform->SetAngle(angle);
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
	Turn(elapsed_time, vx, vz, turn_speed * speed_rate);
}

void EnemyComponent::SetRandomTargetPosition()
{
	float theta = MyMathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = MyMathf::RandomRange(0.0f, territory_range);
	this->target_position.x = territory_origin.x + sinf(theta) * range;
	this->target_position.y = territory_origin.y;
	this->target_position.z = territory_origin.z + cosf(theta) * range;
}

#ifdef _DEBUG

void EnemyComponent::DrawDebugGUI()
{
}

#endif // _DEBUG