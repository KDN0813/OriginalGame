#include "TransformComponent.h"
#include <imgui.h>

#include "Debug/DebugManager.h"

void Transform3DComponent::Update(float elapsed_time)
{
	// 値が変更されていなければ処理しない
	if (!this->change_value) return;

	// ワールド行列の更新
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	DirectX::XMMATRIX W = S * R * T;
	DirectX::XMStoreFloat4x4(&this->transform, W);
	this->change_value = false;

	{
		DebugRenderer* debug_render = DebugManager::Instance()->GetDebugRenderer();
		debug_render->DrawSphere(position, 0.06f, DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f));
	}
}

const DirectX::XMVECTOR Transform3DComponent::AddPositionVec(const DirectX::XMVECTOR& vec)
{
	this->change_value = true;
	DirectX::XMVECTOR ans = DirectX::XMVectorAdd(vec, DirectX::XMLoadFloat3(&this->position));
	DirectX::XMStoreFloat3(&this->position, ans);
	return ans;
}

const DirectX::XMFLOAT3& Transform3DComponent::AddPosition(const DirectX::XMFLOAT3 vec)
{
	this->change_value = true;
	this->position.x += vec.x;
	this->position.y += vec.y;
	this->position.z += vec.z;
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
	if (ImGui::SliderFloat3("angle", &angle_degrees.x, 0, 360.0f));
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

#endif // _DEBUG