#include "Transform2DComponent.h"
#include <imgui.h>
#include "Object/Object.h"
#include <cmath>

#include "Debug/DebugManager.h"

Transform2DComponent::Transform2DComponent(Transform2DParam param)
	:param(param), default_param(param)
{
#ifdef _DEBUG
	this->sphere_world_position = SphereParam(DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), 0.1f);
	SetIsDebugPrimitive(false);
#endif // _DEBUG
}

void Transform2DComponent::Update(float elapsed_time)
{
	// ���[�J���g�����X�t�H�[���̍X�V
	if (this->local_dirty_flag) UpdateLocalParam();

	if (this->world_dirty_flag)
	{
		// ���[���h�g�����X�t�H�[���̍X�V
		UpdateWorldParam();
	}
}

void Transform2DComponent::UpdateWorldParam()
{
	// �e�̍s��擾
	MYMATRIX Parent_transform = DirectX::XMMatrixIdentity();
	if (const auto& owner = GetOwner())
	{
		if (const auto& parent = owner->GetParent())
		{
			if (auto parent_transform = parent->EnsureComponentValid(this->parent_ransform_Wptr))
			{
				Parent_transform = parent_transform->GetWolrdTransform();
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
		// �ʒu�̎擾
		MYVECTOR2 Local_position = this->param.local_position;
		MYVECTOR2 Worldl_position = Parent_transform.Vector2TransformCoord(Local_position);
		Worldl_position.GetFlaot2(this->world_position);

		// �X�P�[���̎擾
		float scaleX = std::sqrt(this->world_transform._11 * this->world_transform._11 + this->world_transform._12 * this->world_transform._12);
		float scaleY = std::sqrt(this->world_transform._21 * this->world_transform._21 + this->world_transform._22 * this->world_transform._22);
		this->world_scale = DirectX::XMFLOAT2(scaleX, scaleY);

		// ��]�p�x�̎擾
		// TODO �e�q�֌W���ł��Ă��Ȃ��̂ŏC������
		this->world_angle = std::atan2(this->world_transform._12 / scaleX, this->world_transform._11 / scaleX);
	}

	this->world_dirty_flag = false;	// �t���O������
}

void Transform2DComponent::UpdateLocalParam()
{
	// ���[�J���s��쐬
	MYMATRIX Local_transform;
	Local_transform.SetLocalMatrix(this->param.local_scale, this->param.local_angle, this->param.local_position);
	Local_transform.GetFlaot4x4(this->local_transform);

	this->local_dirty_flag = false;	// �t���O������
}

const DirectX::XMFLOAT4X4& Transform2DComponent::GetWolrdTransform()
{
	if (this->world_dirty_flag) UpdateWorldParam();
	return this->world_transform;
}

const DirectX::XMFLOAT4X4& Transform2DComponent::GetLocalTransform()
{
	if (this->local_dirty_flag) UpdateLocalParam();
	return this->local_transform;
}

void Transform2DComponent::SetLocalPosition(DirectX::XMFLOAT2 pos) noexcept
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
				if (auto chilled_transform = chilled->GetComponent<Transform2DComponent>())
				{
					chilled_transform->SetWorldDirtyFlag();
				}
			}
		}
	}
}

DirectX::XMFLOAT2 Transform2DComponent::AddLocalPosition(DirectX::XMFLOAT2 vec)
{
	// ���[�J�����W�̍X�V
	MYVECTOR2 Pos = this->param.local_position;
	Pos += vec;
	Pos.GetFlaot2(this->param.local_position);

	// ���[���h����у��[�J���p�����[�^�̍X�V���K�v�ȃt���O�𗧂Ă�
	this->world_dirty_flag = true;
	this->local_dirty_flag = true;

	// �q�I�u�W�F�N�g�̃g�����X�t�H�[���ɒl�X�V���K�v�ȃt���O�𗧂Ă�
	{
		if (const auto& owner = GetOwner())
		{
			for (const auto& chilled : owner->GetChildren())
			{
				if (auto chilled_transform = chilled->GetComponent<Transform2DComponent>())
				{
					chilled_transform->SetWorldDirtyFlag();
				}
			}
		}
	}

	// ���[�J�����W��Ԃ�
	return this->param.local_position;
}

void Transform2DComponent::SetLocalAngle(float angle) noexcept
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
				if (auto chilled_transform = chilled->GetComponent<Transform2DComponent>())
				{
					chilled_transform->SetWorldDirtyFlag();
				}
			}
		}
	}
}

void Transform2DComponent::SetLocalScale(DirectX::XMFLOAT2 scale) noexcept
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
				if (auto chilled_transform = chilled->GetComponent<Transform2DComponent>())
				{
					chilled_transform->SetWorldDirtyFlag();
				}
			}
		}
	}
}

DirectX::XMFLOAT2 Transform2DComponent::GetWorldPosition()
{
	if (this->world_dirty_flag) UpdateWorldParam();
	return this->world_position;
}

DirectX::XMFLOAT2 Transform2DComponent::GetWorldScale()
{
	if (this->world_dirty_flag) UpdateWorldParam();
	return this->world_scale;
}

float Transform2DComponent::GetWorldAngle()
{
	if (this->world_dirty_flag) UpdateWorldParam();
	return world_angle;
}

#ifdef _DEBUG

void Transform2DComponent::DrawDebugGUI()
{
	// local_position�ݒ�
	if (this->param.enable_ndc_debug)
	{
		DirectX::XMFLOAT2 local_position = this->param.local_position;
		if (ImGui::DragFloat2("Local Position", &local_position.x, 0.005f))
		{
			SetLocalPosition(local_position);
		}
		if (ImGui::DragFloat2("Local Scale", &this->param.local_scale.x, 0.1f, 0.0f, 1000.0f))
		{
			this->local_dirty_flag = true;
			this->world_dirty_flag = true;
		}
	}
	else
	{
		DirectX::XMFLOAT2 local_position = this->param.local_position;
		if (ImGui::DragFloat2("Local Position", &local_position.x, 0.1f))
		{
			SetLocalPosition(local_position);
		}
		if (ImGui::DragFloat2("Local Scale", &this->param.local_scale.x, 0.1f))
		{
			this->local_dirty_flag = true;
			this->world_dirty_flag = true;
		}
	}

	float angle_degrees
	{
		DirectX::XMConvertToDegrees(this->param.local_angle)
	};
	if (ImGui::SliderFloat("Local Angle", &angle_degrees, 0, 360.0f))
	{
		this->param.local_angle =
		{
			DirectX::XMConvertToRadians(angle_degrees)
		};
		this->world_dirty_flag = true;
		this->local_dirty_flag = true;
	}

	DirectX::XMFLOAT2 dummy_float2 = this->world_position;
	ImGui::InputFloat2("World Position", &dummy_float2.x);
	dummy_float2 = this->world_scale;
	ImGui::InputFloat2("World Scale", &dummy_float2.x);
	float dummy_float= this->world_angle;
	ImGui::InputFloat("World Angle", &dummy_float);
}

void Transform2DComponent::DrawDebugPrimitive()
{
	DebugManager::Instance debug_manager = DebugManager::GetInstance();
	if (!debug_manager.Get()) return;
	DebugPrimitiveRenderer* debug_render = debug_manager->GetDebugPrimitiveRenderer();
	this->sphere_world_position.SetCenter({ GetWorldPosition().x,GetWorldPosition().y,0.0f });
	debug_render->DrawSphere(this->sphere_world_position);
}

void Transform2DComponent::DrawDebugPrimitiveGUI()
{
	this->sphere_world_position.DrawDebugGUI("Sphere World Position");
}

#endif // _DEBUG