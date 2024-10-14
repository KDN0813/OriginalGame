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

	// �ړI�n�_�܂ł�XZ���ʂł̋�������
	MYVECTOR3 Position = transform->GetPosition();
	MYVECTOR3 Target_position = this->target_position;
	float distSq = (Target_position.GetMyVectorXZ() - Position.GetMyVectorXZ()).LengthSq();
	
	// �ړI�n�_�ֈړ�
	MoveToTarget(elapsed_time, transform, speed_rate);
	
	// �ړI�n�֒�����
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
	// �i�s�x�N�g����0�ȉ��Ȃ珈�����Ȃ�
	float length = sqrtf(vx * vx + vz * vz);
	if (length < 0.001) return;

	auto owner = GetOwner();
	if (auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr))
	{
		speed *= elapsed_time;

		// �P�ʃx�N�g����
		vx /= length;
		vz /= length;

		// ���g�̉�]�l����O���������߂�
		DirectX::XMFLOAT3 angle = transform->GetAngle();
		float frontX = sinf(angle.y);
		float frontZ = cosf(angle.y);

		// ��]�p�����߂邽�߁A2�̒P�ʃx�N�g���̓��ς��v�Z����
		float dot = (frontX * vx + frontZ * vz);

		// ���ϒl��-1.0~1.0�ŕ\������Ă���A2�̒P�ʃx�N�g���̊p�x��
		// �������قǁA1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
		float rot = 1.0f - dot;
		if (rot > speed) rot = speed;

		// ���E������s�����߂�2�̒P�ʃx�N�g���̊O�ς��v�Z����
		float cross = (frontZ * vx) - (frontX * vz);

		// 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
		//  ���E������s�����Ƃɂ���č��E��]��I������
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
	// �^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
	MYVECTOR3 Position = transform->GetPosition();
	MYVECTOR3 Target_position = this->target_position;
	MYVECTOR3 Vec = (Target_position.GetMyVectorXZ() - Position.GetMyVectorXZ()).Normalize();
	float vx = Vec.GetX();
	float vz = Vec.GetZ();

	// �ړ�����
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