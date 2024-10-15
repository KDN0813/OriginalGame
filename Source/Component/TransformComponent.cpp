#include "TransformComponent.h"
#include <imgui.h>

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
	if (!this->change_value) return;

	UpdateTransform();
}

void Transform3DComponent::UpdateTransform()
{
	// ワールド行列の更新
	MYMATRIX W;
	W.SetLocalMatrix(this->scale, this->angle, this->position);
	W.GetFlaot4x4(this->transform);
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