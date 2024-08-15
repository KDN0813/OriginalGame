#include "TransformComponent.h"
#include <imgui.h>

void Transform3DComponent::Update(float elapsed_time)
{
	// 値が変更されていなければ処理しない
	if (!this->change_value) return;

	// ワールド行列の更新
	DirectX::XMVECTOR Q = DirectX::XMLoadFloat4(&rotation);
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(Q);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	DirectX::XMMATRIX W = S * R * T;
	DirectX::XMStoreFloat4x4(&transform, W);
	this->change_value = false;
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
	ImGui::InputFloat3("position", &this->position.x);
	ImGui::InputFloat3("rotation", &this->rotation.x);
	ImGui::InputFloat3("scale", &this->scale.x);
}

#endif // _DEBUG