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

    // ���x�v�Z
    this->velocity =
    {
        this->move_vec.x * this->speed * elapsed_time,
        this->move_vec.y * this->speed * elapsed_time,
        this->move_vec.z * this->speed * elapsed_time,
    };

    if(is_stage_raycas)
    {
		auto gravity = owner->EnsureComponentValid<GravityComponent>(this->gravity_Wptr);
		if (!gravity) return;

		auto stage_object = GameObject::Instance()->GetGameObject(GameObject::OBJECT_TYPE::STAGE);
		if (!stage_object) return;
		auto stage_model = stage_object->EnsureComponentValid<ModelComponent>(this->stage_model_Wptr);

		// Y���̉������Ɍ����ă��C�L���X�g���s��
		{
			// ���݂̈ʒu
			const DirectX::XMFLOAT3 current_pos = transform->GetPosition();

			// ���������̈ړ���
			float my = this->velocity.y;

			if (my < 0.0f)
			{
				// ���C�̊J�n�ʒu�ƏI�_�ʒu
				DirectX::XMFLOAT3 start = { current_pos.x,current_pos.y + step_offset,current_pos.z };
				DirectX::XMFLOAT3 end = { current_pos.x,current_pos.y + my,current_pos.z };

#ifdef _DEBUG	// �f�o�b�O�v���~�e�B�u�\��
				{
					DebugRenderer* debug_render = DebugManager::Instance()->GetDebugRenderer();
					debug_render->DrawSphere(start, 0.05f, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
					debug_render->DrawSphere(end, 0.05f, DirectX::XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f));
				}
#endif // _DEBUG	�f�o�b�O�v���~�e�B�u�\��

				// ���C�L���X�g�ɂ��n�ʔ���
				HitResult hit;
				if (Collision::IntersectRayVsModel(start, end, stage_model.get(), hit))
				{
					transform->SetPosition(hit.position);
					gravity->SetIsGrounded(true);
				}
				else
				{
					DirectX::XMFLOAT3 position = current_pos;
					position.y += my;
					transform->SetPosition(position);

					gravity->SetIsGrounded(false);
				}
			}
		}

		// �O�������Ƀ��C�L���X�g���s��
		{
			// ���݂̈ʒu
			const DirectX::XMFLOAT3 current_pos = transform->GetPosition();

			float velocity_lengthXZ = sqrtf(this->velocity.x * this->velocity.x + this->velocity.z * this->velocity.z);
			if (velocity_lengthXZ > 0.0f)
			{
				// ���������̈ړ���
				float mx = this->velocity.x;
				float mz = this->velocity.z;

				// ���C�̊J�n�ʒu�ƏI�_�ʒu
				DirectX::XMFLOAT3 start = { current_pos.x,current_pos.y + step_offset,current_pos.z };
				DirectX::XMFLOAT3 end = { current_pos.x + mx,current_pos.y + step_offset,current_pos.z + mz };

#ifdef _DEBUG
				// �f�o�b�O�v���~�e�B�u�\��
				{
					DebugRenderer* debug_render = DebugManager::Instance()->GetDebugRenderer();
					debug_render->DrawSphere(start, 0.05f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
					debug_render->DrawSphere(end, 0.05f, DirectX::XMFLOAT4(0.0f, 0.0f, 0.5f, 1.0f));
				}
#endif // _DEBUG

				// ���C�L���X�g�ǔ���
				HitResult hit;
				if (Collision::IntersectRayVsModel(start, end, stage_model.get(), hit))
				{
					// �ǂ��烌�C�̏I�_�܂ł̃x�N�g��
					DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
					DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
					DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

					// �ǂ̖@��
					DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

					// ���˃x�N�g����@���Ɏˉe
					DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);
					Dot = DirectX::XMVectorScale(Dot, 1.1f);

					// �␳�ʒu�̌v�Z
					DirectX::XMVECTOR CorrectionPositon = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
					DirectX::XMFLOAT3 correction_positon;
					DirectX::XMStoreFloat3(&correction_positon, CorrectionPositon);

					// �ǂ�������ւ̃��C�L���X�g
					HitResult hit2;
					if (!Collision::IntersectRayVsModel(start, correction_positon, stage_model.get(), hit2))
					{
						DirectX::XMFLOAT3 positon = current_pos;
						positon.x = correction_positon.x;
						positon.z = correction_positon.z;
						transform->SetPosition(positon);
					}
					else
					{
						DirectX::XMFLOAT3 positon = current_pos;
						positon.x = hit2.position.x;
						positon.z = hit2.position.z;
						transform->SetPosition(positon);
					}
				}
				else
				{
					DirectX::XMFLOAT3 positon = current_pos;
					positon.x += mx;
					positon.z += mz;
					transform->SetPosition(positon);
				}
			}
		}
    }
	else
    {
        transform->AddPosition(this->velocity);
    }
}

bool MovementComponent::IsMoveXZAxis()
{
    return (this->move_vec.x != 0.0f || this->move_vec.z != 0.0f);
}

#ifdef _DEBUG

void MovementComponent::DrawDebugGUI()
{
    ImGui::InputFloat3("Move Vec", &this->move_vec.x);
    ImGui::InputFloat("Speed", &this->speed);
	if (this->is_stage_raycas)
	{
		ImGui::InputFloat("Step Offset", &this->step_offset);
	}
}

#endif _DEBUG