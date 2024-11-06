#include "TransformComponent.h"
#include <imgui.h>
#include "Object/Object.h"

#include "Debug/DebugManager.h"

Transform3DComponent::Transform3DComponent(Transform3DParam param)
	:param(param),default_param(param)
{
	this->change_value = true;
#ifdef _DEBUG
	sphere_world_position = SphereParam(DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), 0.1f);
	SetIsDebugPrimitive(false);
#endif // _DEBUG
}

void Transform3DComponent::Update(float elapsed_time)
{
	// 値が変更されていなければ処理しない
	if (!GetChangeValue()) return;

	UpdateTransform();
}

void Transform3DComponent::UpdateTransform()
{
	// 親の行列取得
	MYMATRIX Parent_transform = DirectX::XMMatrixIdentity();
	std::shared_ptr<Transform3DComponent> parent_transform = nullptr;

	auto owner = GetOwner();
	if (owner)
	{
		auto parent = owner->GetParent();
		if (parent)
		{
			parent_transform = parent->EnsureComponentValid(this->parent_ransform_Wptr);
			if (parent_transform)
			{
				Parent_transform = parent_transform->GetWolrdTransform();
			}
		}
	}

	// ローカル行列作成
	MYMATRIX Local_transform;
	Local_transform.SetLocalMatrix(this->param.local_scale, this->param.local_angle, this->param.local_position);

	// ワールド行列作成
	MYMATRIX World_transform;
	World_transform = Local_transform * Parent_transform;

	Local_transform.GetFlaot4x4(this->local_transform);
	World_transform.GetFlaot4x4(this->world_transform);

	// ワールドポジションの設定
	SetWorldPosition(parent_transform);

	this->change_value = false;
}

const DirectX::XMFLOAT4X4& Transform3DComponent::GetWolrdTransform()
{
	if(!GetChangeValue()) return this->world_transform;
	UpdateTransform();
	return this->world_transform;
}

DirectX::XMFLOAT3 Transform3DComponent::AddLocalPosition(DirectX::XMFLOAT3 vec)
{
	this->change_value = true;
	MYVECTOR3 Pos = this->param.local_position;
	Pos += vec;
	Pos.GetFlaot3(this->param.local_position);
	return this->param.local_position;
}

DirectX::XMFLOAT3 Transform3DComponent::GetWorldPosition()
{
	if (!GetChangeValue()) 
		return this->world_position;
	UpdateTransform();
	return this->world_position;
}

void Transform3DComponent::SetWorldPosition()
{
	auto owner = GetOwner();
	if (!owner)
	{
		this->world_position = this->param.local_position;;
		return;
	};
	auto parent = owner->GetParent();
	if (!parent)
	{
		this->world_position = this->param.local_position;;
		return;
	};
	SetWorldPosition(parent->EnsureComponentValid(this->parent_ransform_Wptr));
}

void Transform3DComponent::SetWorldPosition(std::shared_ptr<Transform3DComponent> parent_transform)
{
	if (!parent_transform)
	{
		this->world_position = this->param.local_position;;
		return;
	};
	MYVECTOR3 Local_position = this->param.local_position;
	MYVECTOR3 Worldl_position = this->param.local_position;
	MYMATRIX World_transform = parent_transform->GetWolrdTransform();

	Worldl_position = World_transform.Vector3TransformCoord(Local_position);
	Worldl_position.GetFlaot3(this->world_position);
}

bool Transform3DComponent::GetChangeValue()
{
	if (this->change_value) return this->change_value;
	auto owner = GetOwner();
	if (!owner) return this->change_value;
	auto parent = owner->GetParent();
	if (!parent) return this->change_value;
	auto parent_transform = parent->EnsureComponentValid(this->parent_ransform_Wptr);
	if(!parent_transform) return this->change_value;
	return parent_transform->GetChangeValue();
}

#ifdef _DEBUG

void Transform3DComponent::DrawDebugGUI()
{
	if (ImGui::InputFloat3("Local Position", &this->param.local_position.x))
	{
		this->change_value = true;
	}
	if (ImGui::InputFloat3("Local Scale", &this->param.local_scale.x))
	{
		this->change_value = true;
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
		this->change_value = true;
	}

	if (ImGui::InputFloat3("World Position", &this->world_position.x))
	{
		this->change_value = true;
	}
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