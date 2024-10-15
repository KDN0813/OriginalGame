#include <imgui.h>
#include "MovementComponent.h"
#include "Object/Object.h"
#include "Object/GameObject.h"
#include "Collision/Collision.h"
#ifdef _DEBUG
#include "Debug/DebugManager.h"

#endif // _DEBUG

#include "Component/TransformComponent.h"
#include "Component/GravityComponent.h"

MovementComponent::MovementComponent()
{
#ifdef _DEBUG
	SetIsDebugPrimitive(false);

#endif // _DEBUG
}

void MovementComponent::Update(float elapsed_time)
{
    auto owner = GetOwner();
	if (!owner) return;
	auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr);
	if (!transform) return;

	MYVECTOR3 Acceleration = this->acceleration;
	float lengthXZ_sq = Acceleration.LengthXZSq();
	float max_accelerationXZ_sq = this->max_accelerationXZ * max_accelerationXZ;
	
	// ���x�v�Z
	if (max_accelerationXZ_sq < lengthXZ_sq)
	{
		float lengthXZ = static_cast<float>(sqrt(lengthXZ_sq));
		MYVECTOR3 Velocity = this->acceleration;
		Velocity = Velocity / lengthXZ * this->max_accelerationXZ * elapsed_time;
		Velocity.GetFlaot3(this->velocity);
	}
	else
	{
		MYVECTOR3 Velocity = this->acceleration;
		Velocity *= elapsed_time;
		Velocity.GetFlaot3(this->velocity);
	}
	this->velocity.y = this->acceleration.y * elapsed_time;

	RaycasVsStage(owner, transform);

	FaceMovementDirection(elapsed_time);

	// �����x��������
	this->acceleration = {};
}

void MovementComponent::FaceMovementDirection(float elapsed_time)
{
	float vx = this->velocity.x;
	float vz = this->velocity.z;
	float speed = this->turn_speed;

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

bool MovementComponent::IsMoveXZAxis() const
{
    return (this->velocity.x != 0.0f || this->velocity.z != 0.0f);
}

void MovementComponent::AddAcceleration(MYVECTOR3 Add_acc)
{
	MYVECTOR3 Acceleration = this->acceleration;
	Acceleration += Add_acc;
	Acceleration.GetFlaot3(this->acceleration);
}

void MovementComponent::AddAccelerationXZ(float x, float z)
{
	MYVECTOR3 Acceleration = this->acceleration;
	Acceleration.AddXZ(x,z);
	Acceleration.GetFlaot3(this->acceleration);
}

void MovementComponent::AddAccelerationX(float x)
{
	MYVECTOR3 Acceleration = this->acceleration;
	Acceleration.AddX(x);
	Acceleration.GetFlaot3(this->acceleration);
}

void MovementComponent::AddAccelerationY(float y)
{
	MYVECTOR3 Acceleration = this->acceleration;
	Acceleration.AddY(y);
	Acceleration.GetFlaot3(this->acceleration);
}

void MovementComponent::AddAccelerationZ(float z)
{
	MYVECTOR3 Acceleration = this->acceleration;
	Acceleration.AddZ(z);
	Acceleration.GetFlaot3(this->acceleration);
}

void MovementComponent::RaycasVsStage(std::shared_ptr<Object> owner,std::shared_ptr<Transform3DComponent>& transform)
{
	if (!this->is_stage_raycas)
	{
		transform->AddPosition(this->velocity);
		return;
	}
	auto stage_object = GameObject::Instance()->GetGameObject(GameObject::OBJECT_TYPE::STAGE);
	if (!stage_object)
	{
		transform->AddPosition(this->velocity);
		return;
	}
	auto stage_model = stage_object->EnsureComponentValid<ModelComponent>(this->stage_model_Wptr);
	if (!stage_model)
	{
		transform->AddPosition(this->velocity);
		return;
	}

	auto gravity = owner->EnsureComponentValid<GravityComponent>(this->gravity_Wptr);
	// �n�ʕ����Ƀ��C�L���X�g���s��
	if (gravity)
	{
		// Y���̉������Ɍ����ă��C�L���X�g���s��
		{
			// ���݂̈ʒu
			const MYVECTOR3 Current_pos = transform->GetWorldPosition();

			// ���������̈ړ���
			float my = this->velocity.y;

			if (my < 0.0f)
			{
				// ���C�̊J�n�ʒu�ƏI�_�ʒu
				MYVECTOR3 Start = Current_pos + MYVECTOR3(0.0f, step_offset, 0.0f);
				MYVECTOR3 End = Current_pos + MYVECTOR3(0.0f, my, 0.0f);

#ifdef _DEBUG	// �f�o�b�O�v���~�e�B�u�\���p�ϐ��̍X�V
				{
					Start.GetFlaot3(this->rayY_start_pos);
					End.GetFlaot3(this->rayY_end_pos);
				}
#endif // _DEBUG	�f�o�b�O�v���~�e�B�u�\��

				// ���C�L���X�g�ɂ��n�ʔ���
				HitResult hit;
				if (Collision::IntersectRayVsModel(Start, End, stage_model.get(), hit))
				{
					transform->SetPosition(hit.position);
					gravity->SetIsGrounded(true);
				}
				else
				{
					DirectX::XMFLOAT3 position{};
					Current_pos.GetFlaot3(position);
					position.y += my;
					transform->SetPosition(position);

					gravity->SetIsGrounded(false);
				}
			}
		}
	}

	// �O�������Ƀ��C�L���X�g���s��
	{
		// ���݂̈ʒu
		const DirectX::XMFLOAT3& current_pos = transform->GetWorldPosition();
		const MYVECTOR3 Current_pos = current_pos;

		float velocity_lengthXZ = MYVECTOR3(this->velocity).LengthXZ();
		if (velocity_lengthXZ > 0.0f)
		{
			// ���������̈ړ���
			float mx = this->velocity.x;
			float mz = this->velocity.z;

			// ���C�̊J�n�ʒu�ƏI�_�ʒu
			MYVECTOR3 Start = Current_pos + MYVECTOR3(0.0f, step_offset, 0.0f);
			MYVECTOR3 End = Current_pos + MYVECTOR3(mx, step_offset, mz);

#ifdef _DEBUG
			// �f�o�b�O�v���~�e�B�u�\���p�ϐ��̍X�V
			{
				Start.GetFlaot3(this->rayXZ_start_pos);
				End.GetFlaot3(this->rayXZ_end_pos);
			}
#endif // _DEBUG

			// ���C�L���X�g�ǔ���
			HitResult hit;
			if (Collision::IntersectRayVsModel(Start, End, stage_model.get(), hit))
			{
				// �ǂ��烌�C�̏I�_�܂ł̃x�N�g��
				MYVECTOR3 S = hit.position;
				MYVECTOR3 VecSE = End - S;

				// �ǂ̖@��
				MYVECTOR3 Normal = hit.normal;

				// ���˃x�N�g����@���Ɏˉe
				MYVECTOR3 Dot = (VecSE.Negate()).DotVec3(Normal) * 1.1f;

				// �␳�ʒu�̌v�Z
				MYVECTOR3 Correction_positon = Normal * Dot + End;
				
				// �ǂ�������ւ̃��C�L���X�g
				HitResult hit2;
				if (!Collision::IntersectRayVsModel(Start, Correction_positon, stage_model.get(), hit2))
				{
					DirectX::XMFLOAT3 pos = {};
					Correction_positon.GetFlaot3(pos);
					pos.y = current_pos.y;
					transform->SetPosition(pos);
				}
				else
				{
					DirectX::XMFLOAT3 pos = { hit2.position.x, current_pos.y, hit2.position.z };
					transform->SetPosition(pos);
				}
			}
			else
			{
				MYVECTOR3 Positon = Current_pos;
				Positon.AddXZ(mx, mz);
				DirectX::XMFLOAT3 position{};
				Positon.GetFlaot3(position);
				transform->SetPosition(position);
			}
		}
	}
}

#ifdef _DEBUG

void MovementComponent::DrawDebugGUI()
{
	ImGui::InputFloat3("Additional Velocity", &this->acceleration.x);
    ImGui::InputFloat("Max AccelerationXZ", &this->max_accelerationXZ);
	ImGui::InputFloat3("Velocity", &this->velocity.x);
	ImGui::InputFloat("Turn Speed", &this->turn_speed);
	ImGui::Checkbox("Is Stage Raycas", &this->is_stage_raycas);
	if (this->is_stage_raycas)
	{
		ImGui::InputFloat("Step Offset", &this->step_offset);
	}
}

void MovementComponent::DrawDebugPrimitive()
{
	DebugPrimitiveRenderer* debug_render = DebugManager::Instance()->GetDebugRenderer();
	debug_render->DrawSphere(this->rayY_start_pos, 0.05f, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
	debug_render->DrawSphere(this->rayY_end_pos, 0.05f, DirectX::XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f));

	debug_render->DrawSphere(this->rayXZ_start_pos, 0.05f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
	debug_render->DrawSphere(this->rayXZ_end_pos, 0.05f, DirectX::XMFLOAT4(0.0f, 0.0f, 0.5f, 1.0f));
}

#endif _DEBUG