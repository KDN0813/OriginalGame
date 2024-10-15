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
	//if (!this->change_value) return;

	UpdateTransform();
}

void Transform3DComponent::UpdateTransform()
{
	// 親の行列取得
	MYMATRIX Parent_transform;
	bool is_parent = true;

	auto owner = GetOwner();
	if (!owner) return;
	auto parent = owner->GetParent();
	if (parent)
	{
		auto parent_transform = parent->GetComponent<Transform3DComponent>();
		if (parent_transform)
		{
			Parent_transform = parent_transform->GetWolrdTransform();
		}
		else
		{
			Parent_transform = DirectX::XMMatrixIdentity();
		}
	}
	else
	{
		Parent_transform = DirectX::XMMatrixIdentity();
	}

	// ローカル行列作成
	MYMATRIX Local_transform;
	Local_transform.SetLocalMatrix(this->scale, this->angle, this->position);

	// ワールド行列作成
	MYMATRIX World_transform;
	World_transform = Local_transform * Parent_transform;

	Local_transform.GetFlaot4x4(this->local_transform);
	World_transform.GetFlaot4x4(this->world_transform);
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
	debug_render->DrawSphere(position, 0.06f, DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f));
}

#endif // _DEBUG