#include <imgui.h>
#include "MovementComponent.h"
#include "Object/Object.h"
#include "Object/GameObject.h"
#include "Collision/Collision.h"
#include "GridObjectManager\GridObjectManager.h"

#ifdef _DEBUG
#include "Debug/DebugManager.h"
#include "Input\Input.h"
#include "Component/PlayerComponent.h"
#endif // _DEBUG

#include "Component/TransformComponent.h"
#include "Component/GravityComponent.h"

MovementComponent::MovementComponent(MovementParam param)
	:param(param),default_param(param)
{
#ifdef _DEBUG
	SetIsDebugPrimitive(false);
	this->rayY_start_pos = SphereParam(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), 0.05f);
	this->rayY_end_pos = SphereParam(DirectX::XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f), 0.05f);
	
	this->rayXZ_start_pos = SphereParam(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), 0.05f);
	this->rayXZ_end_pos = SphereParam(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f), 0.05f);
#endif // _DEBUG
}

void MovementComponent::Update(float elapsed_time)
{
    auto owner = GetOwner();
	if (!owner) return;
	auto transform = owner->GetComponent<Transform3DComponent>(this->transform_Wptr);
	if (!transform) return;

	MYVECTOR3 Acceleration = this->param.acceleration;
	float lengthXZ_sq = Acceleration.LengthXZSq();
	float max_accelerationXZ_sq = this->param.max_accelerationXZ * this->param.max_accelerationXZ;
#ifdef _DEBUG
	if (Input::Instance input = Input::GetInstance(); input.Get())
	{
		if (input->GetGamePad().GetTriggerL())
		{
			max_accelerationXZ_sq = PlayerComponent::debug_move_speed * PlayerComponent::debug_move_speed;
		}
	}
#endif // _DEBUG


	// ���x�v�Z
	if (max_accelerationXZ_sq < lengthXZ_sq)
	{
		float lengthXZ = static_cast<float>(sqrt(lengthXZ_sq));
		MYVECTOR3 Velocity = this->param.acceleration;
		Velocity = Velocity / lengthXZ * this->param.max_accelerationXZ * elapsed_time;
		Velocity.GetFlaot3(this->param.velocity);
	}
	else
	{
		MYVECTOR3 Velocity = this->param.acceleration;
		Velocity *= elapsed_time;
		Velocity.GetFlaot3(this->param.velocity);
	}
	this->param.velocity.y = this->param.acceleration.y * elapsed_time;

	FaceMovementDirection(elapsed_time);

	ResolveGridCellCollision(owner, transform,elapsed_time);

	RaycasVsStage(owner, transform);

	// �ړ���ɂ�����x�o�^����
	GridObjectManager::Instance grid_object_manager = GridObjectManager::GetInstance();
	grid_object_manager->RegisterObject(owner, transform->GetWorldPosition());

	// �����x��������
	this->param.acceleration = {};
}

void MovementComponent::FaceMovementDirection(float elapsed_time)
{
	float vx = this->param.velocity.x;
	float vz = this->param.velocity.z;
	float speed = this->param.turn_speed;

	// �i�s�x�N�g����0�ȉ��Ȃ珈�����Ȃ�
	float length = sqrtf(vx * vx + vz * vz);
	if (length < 0.001) return;

	auto owner = GetOwner();
	if (auto transform = owner->GetComponent<Transform3DComponent>(this->transform_Wptr))
	{
		speed *= elapsed_time;

		// �P�ʃx�N�g����
		vx /= length;
		vz /= length;

		// ���g�̉�]�l����O���������߂�
		DirectX::XMFLOAT3 angle = transform->GetLocalAngle();
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

		transform->SetLocalAngle(angle);
	}
}

bool MovementComponent::IsMoveXZAxis() const
{
    return (this->param.velocity.x != 0.0f || this->param.velocity.z != 0.0f);
}

void MovementComponent::AddAcceleration(MYVECTOR3 Add_acc)
{
	MYVECTOR3 Acceleration = this->param.acceleration;
	Acceleration += Add_acc;
	Acceleration.GetFlaot3(this->param.acceleration);
}

void MovementComponent::AddAccelerationXZ(float x, float z)
{
	MYVECTOR3 Acceleration = this->param.acceleration;
	Acceleration.AddXZ(x,z);
	Acceleration.GetFlaot3(this->param.acceleration);
}

void MovementComponent::AddAccelerationX(float x)
{
	MYVECTOR3 Acceleration = this->param.acceleration;
	Acceleration.AddX(x);
	Acceleration.GetFlaot3(this->param.acceleration);
}

void MovementComponent::AddAccelerationY(float y)
{
	MYVECTOR3 Acceleration = this->param.acceleration;
	Acceleration.AddY(y);
	Acceleration.GetFlaot3(this->param.acceleration);
}

void MovementComponent::AddAccelerationZ(float z)
{
	MYVECTOR3 Acceleration = this->param.acceleration;
	Acceleration.AddZ(z);
	Acceleration.GetFlaot3(this->param.acceleration);
}

void MovementComponent::RaycasVsStage(std::shared_ptr<Object> owner,std::shared_ptr<Transform3DComponent>& transform)
{
	if (!this->param.is_stage_raycas)
	{
		transform->AddLocalPosition(this->param.velocity);
		return;
	}
	GameObject::Instance game_object = GameObject::GetInstance();
	if (!game_object.Get()) return;

	
	std::shared_ptr<ModelComponent> stage_foor_model = nullptr;
	if (auto stage_foor_object = game_object->GetStageFoor())
	{
		stage_foor_model = stage_foor_object->GetComponent<ModelComponent>(this->stage_foor_model_Wptr);
	}

	std::shared_ptr<ModelComponent> stage_wall_model = nullptr;
	if (auto stage_wall_object = game_object->GetStageWall())
	{
		stage_wall_model = stage_wall_object->GetComponent<ModelComponent>(this->stage_wall_model_Wptr);
	}
	
	auto gravity = owner->GetComponent<GravityComponent>(this->gravity_Wptr);
	// �n�ʕ����Ƀ��C�L���X�g���s��
	if (gravity && stage_foor_model)
	{
		// Y���̉������Ɍ����ă��C�L���X�g���s��
		{
			// ���݂̈ʒu
			const MYVECTOR3 Current_pos = transform->GetWorldPosition();

			// ���������̈ړ���
			float my = this->param.velocity.y;

			if (my < 0.0f)
			{
				// ���C�̊J�n�ʒu�ƏI�_�ʒu
				MYVECTOR3 Start = Current_pos + MYVECTOR3(0.0f, this->param.step_offset, 0.0f);
				MYVECTOR3 End = Current_pos + MYVECTOR3(0.0f, my, 0.0f);

#ifdef _DEBUG	// �f�o�b�O�v���~�e�B�u�\���p�ϐ��̍X�V
				{
					DirectX::XMFLOAT3 start_pos{};
					Start.GetFlaot3(start_pos);
					this->rayY_start_pos.SetCenter(start_pos);
					DirectX::XMFLOAT3 end_pos{};
					End.GetFlaot3(end_pos);
					this->rayY_end_pos.SetCenter(end_pos);
				}
#endif // _DEBUG	�f�o�b�O�v���~�e�B�u�\��

				// ���C�L���X�g�ɂ��n�ʔ���
				RayHitResult hit;
				if (Collision::IntersectRayVsModel(Start, End, stage_foor_model.get(), hit))
				{
					transform->SetLocalPosition(hit.position);
					gravity->SetIsGrounded(true);
				}
				else
				{
					DirectX::XMFLOAT3 position{};
					Current_pos.GetFlaot3(position);
					position.y += my;
					transform->SetLocalPosition(position);

					gravity->SetIsGrounded(false);
				}
			}
		}
	}

	// �O�������Ƀ��C�L���X�g���s��
	if(stage_wall_model)
	{
		// ���݂̈ʒu
		const DirectX::XMFLOAT3& current_pos = transform->GetWorldPosition();
		const MYVECTOR3 Current_pos = current_pos;

		float velocity_lengthXZ = MYVECTOR3(this->param.velocity).LengthXZ();
		
#ifdef _DEBUG
		// �f�o�b�O�v���~�e�B�u�\���p�ϐ��̍X�V
		{
			MYVECTOR3 Start = Current_pos + MYVECTOR3(0.0f, this->param.step_offset, 0.0f);

			DirectX::XMFLOAT3 start_pos{};
			Start.GetFlaot3(start_pos);
			this->rayXZ_start_pos.SetCenter(start_pos);
			this->rayXZ_end_pos.SetCenter(start_pos);		// �ړ����ĂȂ��ꍇ���I�_��ύX���邽��start_pos�������Ă���
		}
#endif // _DEBUG

		if (velocity_lengthXZ > 0.0f)
		{
			// ���������̈ړ���
			float mx = this->param.velocity.x;
			float mz = this->param.velocity.z;
			const float adjustment_rate = 0.01f;

			// ���C�̊J�n�ʒu�ƏI�_�ʒu
			MYVECTOR3 Start = Current_pos + MYVECTOR3(-mx * adjustment_rate, this->param.step_offset, -mz * adjustment_rate);
			MYVECTOR3 End = Current_pos + MYVECTOR3(mx, this->param.step_offset, mz);

#ifdef _DEBUG
			// �f�o�b�O�v���~�e�B�u�\���p�ϐ��̍X�V
			{
				DirectX::XMFLOAT3 end_pos{};
				End.GetFlaot3(end_pos);
				this->rayXZ_end_pos.SetCenter(end_pos);
			}
#endif // _DEBUG

			// ���C�L���X�g�ǔ���
			RayHitResult hit;
			if (Collision::IntersectRayVsModel(Start, End, stage_wall_model.get(), hit))
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
				RayHitResult hit2;
				if (!Collision::IntersectRayVsModel(S, Correction_positon, stage_wall_model.get(), hit2))
				{
					DirectX::XMFLOAT3 pos = {};
					Correction_positon.GetFlaot3(pos);
					pos.y = current_pos.y;
					transform->SetLocalPosition(pos);
				}
				else
				{
					DirectX::XMFLOAT3 pos = { hit2.position.x, current_pos.y, hit2.position.z };
					transform->SetLocalPosition(pos);
				}
			}
			else
			{
				MYVECTOR3 Positon = Current_pos;
				Positon.AddXZ(mx, mz);
				DirectX::XMFLOAT3 position{};
				Positon.GetFlaot3(position);
				transform->SetLocalPosition(position);
			}
		}
	}
}

void MovementComponent::ResolveGridCellCollision(std::shared_ptr<Object> owner, std::shared_ptr<Transform3DComponent>& transform,float elapsed_time)
{
	MYVECTOR3 new_position = transform->GetWorldPosition(); // �ړ���̈ʒu
	MYVECTOR3 current_velocity = this->param.velocity;
	new_position += current_velocity;

	DirectX::XMFLOAT3 position_float3{};
	new_position.GetFlaot3(position_float3);

	GridObjectManager::Instance grid_object_manager = GridObjectManager::GetInstance();
	if (!grid_object_manager->RegisterObject(owner, position_float3))
	{
		// ���ɓo�^����Ă���ꍇ
		this->param.velocity = {};
	}
}

#ifdef _DEBUG

void MovementComponent::DrawDebugGUI()
{
	ImGui::InputFloat3("Additional Velocity", &this->param.acceleration.x);
    ImGui::InputFloat("Max AccelerationXZ", &this->param.max_accelerationXZ);
	ImGui::InputFloat3("Velocity", &this->param.velocity.x);
	ImGui::InputFloat("Turn Speed", &this->param.turn_speed);
	ImGui::Checkbox("Is Stage Raycas", &this->param.is_stage_raycas);
	if (this->param.is_stage_raycas)
	{
		ImGui::InputFloat("Step Offset", &this->param.step_offset);
	}

	auto owner = GetOwner();
	if (!owner) return;
	auto transform = owner->GetComponent<Transform3DComponent>(this->transform_Wptr);
	if (!transform) return;

	GridObjectManager::Instance grid_object_manager = GridObjectManager::GetInstance();
	int cell_index = grid_object_manager->GetCellIndex(transform->GetWorldPosition());
	DirectX::XMFLOAT3 cell_center = grid_object_manager->GetCellCenter(cell_index);

	ImGui::InputInt("cell_index", &cell_index);
	ImGui::InputFloat3("cell_center", &cell_center.x);
}

void MovementComponent::DrawDebugPrimitive()
{
	DebugManager::Instance debug_manager = DebugManager::GetInstance();

	DebugPrimitiveRenderer* debug_render = debug_manager->GetDebugPrimitiveRenderer();
	debug_render->DrawSphere(this->rayY_start_pos);
	debug_render->DrawSphere(this->rayY_end_pos);

	debug_render->DrawSphere(this->rayXZ_start_pos);
	debug_render->DrawSphere(this->rayXZ_end_pos);
}

void MovementComponent::DrawDebugPrimitiveGUI()
{
	this->rayY_start_pos.DrawDebugGUI("RayY Start");
	this->rayY_end_pos.DrawDebugGUI("RayY End");
	this->rayXZ_start_pos.DrawDebugGUI("RayXZ Start");
	this->rayXZ_end_pos.DrawDebugGUI("RayXZ End");
}

#endif _DEBUG