#include "TransformComponent.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG
#include <cmath>
#include "Object/Object.h"

#include "Debug/DebugManager.h"

Transform3DComponent::Transform3DComponent(Transform3DParam param)
	:param(param),default_param(param)
{
#ifdef _DEBUG
	this->sphere_world_position = SphereParam(DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), 0.1f);
	SetIsDebugPrimitive(false);
#endif // _DEBUG
}

void Transform3DComponent::Update(float elapsed_time)
{
	// ���[�J���g�����X�t�H�[���̍X�V
	if (this->local_dirty_flag) UpdateLocalParam();

	if (this->world_dirty_flag)
	{
		// ���[���h�g�����X�t�H�[���̍X�V
		UpdateWorldParam();
	}
}

void Transform3DComponent::UpdateWorldParam()
{
	// �e�̍s��擾
	MYMATRIX Parent_transform = DirectX::XMMatrixIdentity();
	if (const auto& owner = GetOwner())
	{
		if (const auto& parent = owner->GetParent())
		{
			if (auto parent_transform = parent->GetComponent(this->parent_transform_Wptr))
			{
				Parent_transform = parent_transform->GetWorldTransform();
			}
		}
	}

	// ���[�J���g�����X�t�H�[���擾
	if (this->local_dirty_flag) UpdateLocalParam();	// ���[�J���̒l���ύX����Ă���Ȃ烍�[�J���p�����[�^�̍X�V����
	MYMATRIX Local_transform = this->local_transform;

	// ���[���h�s��쐬
	MYMATRIX World_transform = Local_transform * Parent_transform;
	World_transform.GetFlaot4x4(this->world_transform);

	// ���[���h�p�����[�^�̍X�V
	{
		// ���[���h�|�W�V�����̐ݒ�
		UpdateWorldPosition(Parent_transform);
	}

	this->world_dirty_flag = false;	// �t���O������
}

void Transform3DComponent::UpdateLocalParam()
{
	// ���[�J���s��쐬
	MYMATRIX Local_transform;
	Local_transform.SetLocalMatrix(this->param.local_scale, this->param.local_angle, this->param.local_position);
	Local_transform.GetFlaot4x4(this->local_transform);

	this->local_dirty_flag = false;	// �t���O������
}

const DirectX::XMFLOAT4X4& Transform3DComponent::GetWorldTransform()
{
	if(this->world_dirty_flag) UpdateWorldParam();
	return this->world_transform;
}

const DirectX::XMFLOAT4X4& Transform3DComponent::GetLocalTransform()
{
	if (this->local_dirty_flag) UpdateLocalParam();
	return this->local_transform;
}

void Transform3DComponent::SetLocalPosition(DirectX::XMFLOAT3 pos) noexcept
{
	// ���[�J�����W���X�V
	this->param.local_position = pos;
	
	// ���[���h����у��[�J���p�����[�^�̍X�V���K�v�ȃt���O�𗧂Ă�
	this->world_dirty_flag = true;
	this->local_dirty_flag = true;

	// �q�I�u�W�F�N�g�̃g�����X�t�H�[���ɒl�X�V���K�v�ȃt���O�𗧂Ă�
	{
		if (const auto& owner = GetOwner())
		{
			for (const auto& chilled : owner->GetChildren())
			{
				if (auto chilled_transform = chilled->GetComponent<Transform3DComponent>())
				{
					chilled_transform->SetWorldDirtyFlag();
				}
			}
		}
	}
}

DirectX::XMFLOAT3 Transform3DComponent::AddLocalPosition(DirectX::XMFLOAT3 vec)
{
	// ���[�J�����W�̍X�V
	MYVECTOR3 Pos = this->param.local_position;
	Pos += vec;
	Pos.GetFlaot3(this->param.local_position);

	// ���[���h����у��[�J���p�����[�^�̍X�V���K�v�ȃt���O�𗧂Ă�
	this->world_dirty_flag = true;
	this->local_dirty_flag = true;

	// �q�I�u�W�F�N�g�̃g�����X�t�H�[���ɒl�X�V���K�v�ȃt���O�𗧂Ă�
	{
		if (const auto& owner = GetOwner())
		{
			for (const auto& chilled : owner->GetChildren())
			{
				if (auto chilled_transform = chilled->GetComponent<Transform3DComponent>())
				{
					chilled_transform->SetWorldDirtyFlag();
				}
			}
		}
	}

	// ���[�J�����W��Ԃ�
	return this->param.local_position;
}

void Transform3DComponent::SetLocalAngle(DirectX::XMFLOAT3 angle) noexcept
{
	// ���[�J���A���O�����X�V
	this->param.local_angle = angle;

	// ���[���h����у��[�J���p�����[�^�̍X�V���K�v�ȃt���O�𗧂Ă�
	this->world_dirty_flag = true;
	this->local_dirty_flag = true;

	// �q�I�u�W�F�N�g�̃g�����X�t�H�[���ɒl�X�V���K�v�ȃt���O�𗧂Ă�
	{
		if (const auto& owner = GetOwner())
		{
			for (const auto& chilled : owner->GetChildren())
			{
				if (auto chilled_transform = chilled->GetComponent<Transform3DComponent>())
				{
					chilled_transform->SetWorldDirtyFlag();
				}
			}
		}
	}
}

void Transform3DComponent::SetLocalScale(DirectX::XMFLOAT3 scale) noexcept
{
	// ���[�J���A���O�����X�V
	this->param.local_scale = scale;

	// ���[���h����у��[�J���p�����[�^�̍X�V���K�v�ȃt���O�𗧂Ă�
	this->world_dirty_flag = true;
	this->local_dirty_flag = true;

	// �q�I�u�W�F�N�g�̃g�����X�t�H�[���ɒl�X�V���K�v�ȃt���O�𗧂Ă�
	{
		if (const auto& owner = GetOwner())
		{
			for (const auto& chilled : owner->GetChildren())
			{
				if (auto chilled_transform = chilled->GetComponent<Transform3DComponent>())
				{
					chilled_transform->SetWorldDirtyFlag();
				}
			}
		}
	}
}

DirectX::XMFLOAT3 Transform3DComponent::GetWorldPosition()
{
	if (this->world_dirty_flag) UpdateWorldParam();
	return this->world_position;
}

void Transform3DComponent::UpdateWorldPosition()
{
	MYMATRIX Parent_transform = DirectX::XMMatrixIdentity();
	const auto& owner = GetOwner();
	if (!owner)
	{
		this->world_position = this->param.local_position;;
		return;
	};
	const auto& parent = owner->GetParent();
	if (!parent)
	{
		this->world_position = this->param.local_position;;
		return;
	};
	auto parent_ransform = parent->GetComponent(this->parent_transform_Wptr);
	if (!parent_ransform)
	{
		this->world_position = this->param.local_position;;
		return;
	}
	Parent_transform = parent_ransform->GetWorldTransform();
	UpdateWorldPosition(Parent_transform);
}

void Transform3DComponent::UpdateWorldPosition(MYMATRIX Parent_transform)
{
	MYVECTOR3 Local_position = this->param.local_position;

	MYVECTOR3 Worldl_position = Parent_transform.Vector3TransformCoord(Local_position);
	Worldl_position.GetFlaot3(this->world_position);
}

#ifdef _DEBUG

void Transform3DComponent::DrawDebugGUI()
{
	// local_position�ݒ�
	{
		DirectX::XMFLOAT3 local_position = this->param.local_position;
		if (ImGui::DragFloat3("Local Position", &local_position.x, 0.1f))
		{
			SetLocalPosition(local_position);
		}
	}
	if (ImGui::DragFloat3("Local Scale", &this->param.local_scale.x, 0.1f))
	{
		this->local_dirty_flag = true;
		this->world_dirty_flag = true;
	}

	DirectX::XMFLOAT3 angle_degrees
	{
		DirectX::XMConvertToDegrees(this->param.local_angle.x),
		DirectX::XMConvertToDegrees(this->param.local_angle.y),
		DirectX::XMConvertToDegrees(this->param.local_angle.z),
	};
	if (ImGui::SliderFloat3("Local Angle", &angle_degrees.x, 0, 360.0f))
	{
		this->param.local_angle =
		{
			DirectX::XMConvertToRadians(angle_degrees.x),
			DirectX::XMConvertToRadians(angle_degrees.y),
			DirectX::XMConvertToRadians(angle_degrees.z),
		};
		this->world_dirty_flag = true;
		this->local_dirty_flag = true;
	}

	DirectX::XMFLOAT3 dummy_float3 = this->world_position;
	ImGui::InputFloat3("World Position", &dummy_float3.x);
}

void Transform3DComponent::DrawDebugPrimitive()
{
	DebugManager::Instance debug_manager = DebugManager::GetInstance();
	if (!debug_manager.Get()) return;
	DebugPrimitiveRenderer* debug_render = debug_manager->GetDebugPrimitiveRenderer();
	this->sphere_world_position.SetCenter(GetWorldPosition());
	debug_render->DrawSphere(this->sphere_world_position);
}

void Transform3DComponent::DrawDebugPrimitiveGUI()
{
	this->sphere_world_position.DrawDebugGUI("Sphere World Position");
}

#endif // _DEBUG