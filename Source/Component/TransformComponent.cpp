#include "TransformComponent.h"
#include <imgui.h>
#include "Object/Object.h"

#include "Debug/DebugManager.h"

Transform3DComponent::Transform3DComponent()
{
#ifdef _DEBUG
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
	Local_transform.SetLocalMatrix(this->scale, this->angle, this->position);

	// ワールド行列作成
	MYMATRIX World_transform;
	World_transform = Local_transform * Parent_transform;

	Local_transform.GetFlaot4x4(this->local_transform);
	World_transform.GetFlaot4x4(this->world_transform);

	// ワールドポジションの設定
	SetWorldPosition(parent_transform);

	this->change_value = false;
}

DirectX::XMFLOAT3 Transform3DComponent::AddPosition(DirectX::XMFLOAT3 vec)
{
	this->change_value = true;
	MYVECTOR3 Pos = this->position;
	Pos += vec;
	Pos.GetFlaot3(this->position);
	return this->position;
}

DirectX::XMFLOAT3 Transform3DComponent::GetWorldPosition()
{
	if (GetChangeValue()) return this->world_position;
	SetWorldPosition();
	return this->world_position;
}

void Transform3DComponent::SetWorldPosition()
{
	auto owner = GetOwner();
	if (!owner)
	{
		this->world_position = this->position;;
		return;
	};
	auto parent = owner->GetParent();
	if (!parent)
	{
		this->world_position = this->position;;
		return;
	};
	SetWorldPosition(parent->EnsureComponentValid(this->parent_ransform_Wptr));
}

void Transform3DComponent::SetWorldPosition(std::shared_ptr<Transform3DComponent> parent_transform)
{
	if (!parent_transform)
	{
		this->world_position = this->position;;
		return;
	};
	MYVECTOR3 Local_position = this->position;
	MYVECTOR3 Worldl_position = this->position;
	MYMATRIX World_transform = parent_transform->GetWolrdTransform();

	Worldl_position = World_transform.Vector3TransformCoord(Local_position);
	Worldl_position.GetFlaot3(this->world_position);
}

bool Transform3DComponent::GetChangeValue()
{
	auto owner = GetOwner();
	if (!owner) return false;
	auto parent = owner->GetParent();
	if (!parent) return this->change_value;
	auto parent_transform = parent->EnsureComponentValid(this->parent_ransform_Wptr);
	if(!parent_transform) return this->change_value;
	return parent_transform->GetChangeValue();
}

#ifdef _DEBUG

void Transform3DComponent::DrawDebugGUI()
{
	if (ImGui::InputFloat3("position", &this->position.x))
	{
		this->change_value = true;
	}
	if (ImGui::InputFloat3("scale", &this->scale.x))
	{
		this->change_value = true;
	}

	DirectX::XMFLOAT3 angle_degrees
	{
		DirectX::XMConvertToDegrees(this->angle.x),
		DirectX::XMConvertToDegrees(this->angle.y),
		DirectX::XMConvertToDegrees(this->angle.z),
	};
	if (ImGui::SliderFloat3("angle", &angle_degrees.x, 0, 360.0f))
	{
		this->angle =
		{
			DirectX::XMConvertToRadians(angle_degrees.x),
			DirectX::XMConvertToRadians(angle_degrees.y),
			DirectX::XMConvertToRadians(angle_degrees.z),
		};
		this->change_value = true;
	}
}

void Transform3DComponent::DrawDebugPrimitive()
{
	DebugPrimitiveRenderer* debug_render = DebugManager::Instance()->GetDebugRenderer();
	debug_render->DrawSphere(this->world_position, 0.06f, DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f));
}

#endif // _DEBUG