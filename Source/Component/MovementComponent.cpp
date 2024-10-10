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

	// ���Z��
	if (max_accelerationXZ_sq < lengthXZ_sq)
	{
		float lengthXZ = sqrt(lengthXZ_sq);
		this->velocity = this->acceleration / lengthXZ * this->max_accelerationXZ * elapsed_time;
	}
	else
	{
		this->velocity = this->acceleration * elapsed_time;
	}
	this->velocity.SetY(this->acceleration.GetY() * elapsed_time);

	// �X�e�[�W�Ƃ̃��C�L���X�g
    if(is_stage_raycas)
    {
		auto stage_object = GameObject::Instance()->GetGameObject(GameObject::OBJECT_TYPE::STAGE);
		auto stage_model = stage_object->EnsureComponentValid<ModelComponent>(this->stage_model_Wptr);
		if (stage_model && stage_object)
		{
			auto gravity = owner->EnsureComponentValid<GravityComponent>(this->gravity_Wptr);
			// �n�ʕ����Ƀ��C�L���X�g���s��
			if (gravity)
			{
				// Y���̉������Ɍ����ă��C�L���X�g���s��
				{
					// ���݂̈ʒu
					const DirectX::XMFLOAT3 current_pos = transform->GetPosition();

					// ���������̈ړ���
					float my = this->velocity.GetY();

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
			}

			// �O�������Ƀ��C�L���X�g���s��
			{
				// ���݂̈ʒu
				const DirectX::XMFLOAT3 current_pos = transform->GetPosition();

				float velocity_lengthXZ = this->velocity.LengthXZ();
				if (velocity_lengthXZ > 0.0f)
				{
					// ���������̈ړ���
					float mx = this->velocity.GetX();
					float mz = this->velocity.GetZ();

					// ���C�̊J�n�ʒu�ƏI�_�ʒu
					MYVECTOR3 start = { current_pos.x,current_pos.y + step_offset,current_pos.z };
					MYVECTOR3 end = { current_pos.x + mx,current_pos.y + step_offset,current_pos.z + mz };

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
					if (Collision::IntersectRayVsModel(start, end, stage_model.get(), hit))
					{
						// �ǂ��烌�C�̏I�_�܂ł̃x�N�g��
						MYVECTOR3 vecSE = end - hit.position;

						// �ǂ̖@��
						MYVECTOR3& normal = hit.normal;

						// ���˃x�N�g����@���Ɏˉe
						MYVECTOR3 dot = (vecSE.Negate()).Dot(normal) * 1.1f;

						// �␳�ʒu�̌v�Z
						MYVECTOR3 correction_positon = normal * dot + end;

						// �ǂ�������ւ̃��C�L���X�g
						HitResult hit2;
						if (!Collision::IntersectRayVsModel(start, correction_positon, stage_model.get(), hit2))
						{
							MYVECTOR3 positon = correction_positon;
							positon.SetY(current_pos.y);
							transform->SetPosition(positon);
						}
						else
						{
							MYVECTOR3 positon = hit2.position;
							positon.SetY(current_pos.y);
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
    }
	else
    {
        transform->AddPosition(this->velocity.GetFlaot3());
    }

	// �����x��������
	this->acceleration = {};
}

bool MovementComponent::IsMoveXZAxis()
{
    return (this->velocity.GetX() != 0.0f || this->velocity.GetZ() != 0.0f);
}

#ifdef _DEBUG

void MovementComponent::DrawDebugGUI()
{
    //ImGui::InputFloat3("Additional Velocity", &this->acceleration.x);
    ImGui::InputFloat("Max AccelerationXZ", &this->max_accelerationXZ);
    //ImGui::InputFloat3("Velocity", &this->velocity.x);
	ImGui::Checkbox("Is Stage Raycas", &this->is_stage_raycas);
	if (this->is_stage_raycas)
	{
		ImGui::InputFloat("Step Offset", &this->step_offset);
	}
}

#endif _DEBUG