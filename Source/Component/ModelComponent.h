#pragma once
#include <d3d11.h>
#include <vector>
#include <string>
#include <memory>
#include "System/MyMath/MYMATRIX.h"
#include "Component.h"
#include "Model/ModelCommonData.h"

class ModelResource;

class Transform3DComponent;

class ModelComponent : public Component
{
public:
	struct Node
	{
		const char* name = {};
		Node* parent = {};
		DirectX::XMFLOAT3 scale = {};
		DirectX::XMFLOAT4 rotate = {};
		DirectX::XMFLOAT3 translate = {};
		DirectX::XMFLOAT4X4	local_transform = {};
		DirectX::XMFLOAT4X4	world_transform = {};
		std::vector<Node*>	children;
	};

public:
	ModelComponent(ID3D11Device* device, const char* filename);

	// �J�n�֐�
	void Start() override;
	// �X�V�֐�
	void Update(float elapsed_time) override;
	// ���O�擾
	const char* GetName()const override { return "ModelComponent"; };
	// �D��x
	const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::LOW; }

	// �s��v�Z
	void UpdateTransform(MYMATRIX Transform);

	// �m�[�h����
	ModelComponent::Node* FindNode(const char* name);

	// �e��f�[�^�擾
	const std::vector<Node>& GetNodes() const { return node_vec; }
	std::vector<Node>& GetNodes() { return node_vec; }
	const ModelResource* GetResource() const { return resource.get(); }
	std::shared_ptr<ModelResource>& GetResourceSptr() { return resource; }

private:
	std::shared_ptr<ModelResource>	resource;	// ���f�����\�[�X
	std::vector<Node>				node_vec;	// �m�[�h
private:
	std::weak_ptr<Transform3DComponent> transform_Wptr;

#ifdef _DEBUG
public:
	void DrawDebugGUI() override;
private:
	const char* model_filename;
#endif // _DEBUG
};

