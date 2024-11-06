#include <imgui.h>
#include "System/MyMath/MYMATRIX.h"
#include "System/MyMath/MYVECTOR4.h"
#include "Graphics/Graphics.h"
#include "ModelComponent.h"
#include "Object/Object.h"
#include "Model/ModelResourceManager.h"

#include "Component/TransformComponent.h"

ModelComponent::ModelComponent( const char* filename)
{
#ifdef _DEBUG
	this->model_filename = filename;
#endif // _DEBUG

	Graphics::Instance graphics = Graphics::GetInstance();
	if (!graphics.Get()) return;
	ID3D11Device* device = graphics->GetDevice();

	// リソース読み込み
	ModelResourceManager::Instance model_resource_manager = ModelResourceManager::GetInstance();
	if (model_resource_manager.Get())
	{
		this->resource = model_resource_manager->LoadModelResource(device, filename);
	}
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
	UpdateTransform(MYMATRIX());
}

void ModelComponent::Update(float elapsed_time)
{
	MYMATRIX World_transform;
	World_transform.SetIdentity();

	auto owner = GetOwner();
	if (owner)
	{
		if (auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr))
		{
			World_transform = transform->GetWolrdTransform();
		}
	}

	UpdateTransform(World_transform);
}

void ModelComponent::UpdateTransform(MYMATRIX Transform)
{
	for (Node& node : node_vec)
	{
		// ローカル行列算出
		MYMATRIX Local_transform = node.local_transform;
		Local_transform.SetLocalMatrix(node.scale, node.rotate, node.translate);

		// ワールド行列算出
		MYMATRIX Parent_transform;
		if (node.parent != nullptr)
		{
			Parent_transform = node.parent->world_transform;
		}
		else
		{
			Parent_transform = Transform;
		}

		MYMATRIX World_transform = Local_transform * Parent_transform;
		
		Local_transform.GetFlaot4x4(node.local_transform);
		World_transform.GetFlaot4x4(node.world_transform);
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