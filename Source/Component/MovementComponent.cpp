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

void MovementComponent::Update(float elapsed_time)
{
    auto owner = GetOwner();
	if (!owner) return;
	auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr);
	if (!transform) return;

	float lengthXZ_sq = this->acceleration.LengthXZSq();
	float max_accelerationXZ_sq = this->max_accelerationXZ * max_accelerationXZ;
	
	// ���x�v�Z
	if (max_accelerationXZ_sq < lengthXZ_sq)
	{
		float lengthXZ = static_cast<float>(sqrt(lengthXZ_sq));
		this->velocity = this->acceleration / lengthXZ * this->max_accelerationXZ * elapsed_time;
	}
	else
	{
		this->velocity = this->acceleration * elapsed_time;
	}
	this->velocity.SetY(this->acceleration.GetY() * elapsed_time);

	RaycasVsStage(owner, transform);

	// �����x��������
	this->acceleration = {};
}

bool MovementComponent::IsMoveXZAxis()
{
    return (this->velocity.GetX() != 0.0f || this->velocity.GetZ() != 0.0f);
}

void MovementComponent::RaycasVsStage(std::shared_ptr<Object> owner,std::shared_ptr<Transform3DComponent>& transform)
{
	if (!this->is_stage_raycas)
	{
		transform->AddPosition(this->velocity.GetFlaot3());
		return;
	}
	auto stage_object = GameObject::Instance()->GetGameObject(GameObject::OBJECT_TYPE::STAGE);
	if (!stage_object)
	{
		transform->AddPosition(this->velocity.GetFlaot3());
		return;
	}
	auto stage_model = stage_object->EnsureComponentValid<ModelComponent>(this->stage_model_Wptr);
	if (!stage_model)
	{
		transform->AddPosition(this->velocity.GetFlaot3());
		return;
	}

	auto gravity = owner->EnsureComponentValid<GravityComponent>(this->gravity_Wptr);
	// �n�ʕ����Ƀ��C�L���X�g���s��
	if (gravity)
	{
		// Y���̉������Ɍ����ă��C�L���X�g���s��
		{
			// ���݂̈ʒu
			const MYVECTOR3 current_pos = transform->GetPosition();

			// ���������̈ړ���
			float my = this->velocity.GetY();

			if (my < 0.0f)
			{
				// ���C�̊J�n�ʒu�ƏI�_�ʒu
				MYVECTOR3 start = current_pos + MYVECTOR3(0.0f, step_offset, 0.0f);
				MYVECTOR3 end = current_pos + MYVECTOR3(0.0f, my, 0.0f);

#ifdef _DEBUG	// �f�o�b�O�v���~�e�B�u�\��
				{
					DebugRenderer* debug_render = DebugManager::Instance()->GetDebugRenderer();
					debug_render->DrawSphere(start.GetFlaot3(), 0.05f, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
					debug_render->DrawSphere(end.GetFlaot3(), 0.05f, DirectX::XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f));
				}
#endif // _DEBUG	�f�o�b�O�v���~�e�B�u�\��

				// ���C�L���X�g�ɂ��n�ʔ���
				HitResult hit;
				if (Collision::IntersectRayVsModel(start.GetFlaot3(), end.GetFlaot3(), stage_model.get(), hit))
				{
					transform->SetPosition(hit.position);
					gravity->SetIsGrounded(true);
				}
				else
				{
					MYVECTOR3 position = current_pos;
					position.AddY(my);
					transform->SetPosition(position);

					gravity->SetIsGrounded(false);
				}
			}
		}
	}

	// �O�������Ƀ��C�L���X�g���s��
	{
		// ���݂̈ʒu
		const MYVECTOR3 current_pos = transform->GetPosition();

		float velocity_lengthXZ = this->velocity.LengthXZ();
		if (velocity_lengthXZ > 0.0f)
		{
			// ���������̈ړ���
			float mx = this->velocity.GetX();
			float mz = this->velocity.GetZ();

			// ���C�̊J�n�ʒu�ƏI�_�ʒu
			MYVECTOR3 start = current_pos + MYVECTOR3(0.0f, step_offset, 0.0f);
			MYVECTOR3 end = current_pos + MYVECTOR3(0.0f, step_offset, mz);

#ifdef _DEBUG
			// �f�o�b�O�v���~�e�B�u�\��
			{
				DebugRenderer* debug_render = DebugManager::Instance()->GetDebugRenderer();
				debug_render->DrawSphere(start.GetFlaot3(), 0.05f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
				debug_render->DrawSphere(end.GetFlaot3(), 0.05f, DirectX::XMFLOAT4(0.0f, 0.0f, 0.5f, 1.0f));
			}
#endif // _DEBUG

			// ���C�L���X�g�ǔ���
			HitResult hit;
			if (Collision::IntersectRayVsModel(start.GetFlaot3(), end.GetFlaot3(), stage_model.get(), hit))
			{
				// �ǂ��烌�C�̏I�_�܂ł̃x�N�g��
				MYVECTOR3 vecSE = end - hit.position;

				// �ǂ̖@��
				MYVECTOR3 normal = hit.normal;

				// ���˃x�N�g����@���Ɏˉe
				MYVECTOR3 dot = (vecSE.Negate()).Dot(normal) * 1.1f;

				// �␳�ʒu�̌v�Z
				MYVECTOR3 correction_positon = normal * dot + end;

				// �ǂ�������ւ̃��C�L���X�g
				HitResult hit2;
				if (!Collision::IntersectRayVsModel(start.GetFlaot3(), correction_positon.GetFlaot3(), stage_model.get(), hit2))
				{
					MYVECTOR3 positon = MYVECTOR3(correction_positon.GetX(), current_pos.GetY(), correction_positon.GetZ());
					transform->SetPosition(positon);
				}
				else
				{
					MYVECTOR3 positon = MYVECTOR3(hit2.position.x, current_pos.GetY(), hit2.position.z);
					transform->SetPosition(positon);
				}
			}
			else
			{
				MYVECTOR3 positon = current_pos;
				positon.AddXZ(mx, mz);
				transform->SetPosition(positon);
			}
		}
	}
}

#ifdef _DEBUG

void MovementComponent::DrawDebugGUI()
{
	this->acceleration.InputFloat("Additional Velocity");
    ImGui::InputFloat("Max AccelerationXZ", &this->max_accelerationXZ);
	this->velocity.InputFloat("Velocity");
	ImGui::Checkbox("Is Stage Raycas", &this->is_stage_raycas);
	if (this->is_stage_raycas)
	{
		ImGui::InputFloat("Step Offset", &this->step_offset);
	}
}

#endif _DEBUG