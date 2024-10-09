#include <imgui.h>
#include "ModelComponent.h"
#include "Object/Object.h"
#include "Model/ModelResourceManager.h"

#include "Component/TransformComponent.h"

ModelComponent::ModelComponent(ID3D11Device* device, const char* filename)
{
#ifdef _DEBUG
	this->model_filename = filename;
#endif // _DEBUG

	// リソース読み込み
	this->resource = ModelResourceManager::Instance()->LoadModelResource(device, filename);
}

void ModelComponent::Start()
{
	// ノード
	const std::vector<ModelResource::Node>& res_node_vec = this->resource->GetNodes();

	this->node_vec.resize(res_node_vec.size());
	for (size_t node_index = 0; node_index < this->node_vec.size(); ++node_index)
	{
		auto&& src = res_node_vec.at(node_index);
		auto&& dst = node_vec.at(node_index);

		dst.name = src.name.c_str();
		dst.parent = src.parent_index >= 0 ? &node_vec.at(src.parent_index) : nullptr;
		dst.scale = src.scale;
		dst.rotate = src.rotate;
		dst.translate = src.translate;

		if (dst.parent != nullptr)
		{
			dst.parent->children.emplace_back(&dst);
		}
	}

	// 行列計算
	const DirectX::XMFLOAT4X4 transform =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	UpdateTransform(transform);
}

void ModelComponent::Update(float elapsed_time)
{
	auto owner = GetOwner();
	DirectX::XMFLOAT4X4 world_transform;
	if (auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr))
	{
		world_transform = transform->GetTransform();
	}
	else
	{
		DirectX::XMStoreFloat4x4(&world_transform, DirectX::XMMatrixIdentity());
	}

	UpdateTransform(world_transform);
}

void ModelComponent::UpdateTransform(MYMATRIX transform)
{
	for (Node& node : node_vec)
	{
		// ローカル行列算出
		node.local_transform.SetLocalMatrix(node.scale, node.rotate, node.translate);

		// ワールド行列算出
		MYMATRIX parent_transform;
		if (node.parent != nullptr)
		{
			parent_transform = node.parent->world_transform;
		}
		else
		{
			parent_transform = transform;
		}
		node.world_transform = node.local_transform * parent_transform;
	}
}

ModelComponent::Node* ModelComponent::FindNode(const char* name)
{
	for (Node& node : this->node_vec)
	{
		if (0 == std::strcmp(node.name, name))
		{
			return &node;
		}
	}
	return nullptr;
}

#ifdef _DEBUG

void ModelComponent::DrawDebugGUI()
{
	char buffer[1024];
	::strncpy_s(buffer, sizeof(buffer), model_filename, sizeof(buffer));
	ImGui::InputText("Model FileName", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue);
}

#endif // _DEBUG