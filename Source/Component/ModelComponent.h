#pragma once
#include <d3d11.h>
#include <vector>
#include <string>
#include <memory>
#include <DirectXCollision.h>
#include "System/MyMath/MYMATRIX.h"
#include "Component.h"
#include "Model/ModelCommonData.h"
#ifdef _DEBUG
#include "Debug\DebugPrimitiveRenderer.h"
#endif // _DEBUG

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
	ModelComponent(const char* filename);

	// �J�n�֐�
	void Start() override;
	// ���X�^�[�g����
	void ReStart() override {};
	// �X�V�֐�
	void Update(float elapsed_time) override;
	// ���O�擾
	const char* GetName()const override { return "ModelComponent"; };
	// �D��x
	const PRIORITY GetPriority()const noexcept override { return PRIORITY::LOW; }

	// �s��v�Z
	void UpdateTransform(MYMATRIX Transform);

	// �m�[�h����
	ModelComponent::Node* FindNode(const char* name);

	// �e��f�[�^�擾
	const std::vector<Node>& GetNodes() const { return node_vec; }
	std::vector<Node>& GetNodes() { return node_vec; }
	const ModelResource* GetResource() const { return resource.get(); }
	std::shared_ptr<ModelResource>& GetResourceSptr() { return resource; }
	float GetTileCount() const { return this->tile_count; }
	void SetTileCount(float tile_count) { this->tile_count = tile_count; }
	DirectX::BoundingBox GetBoundingBox(size_t index);

private:
	std::shared_ptr<ModelResource>	resource;	// ���f�����\�[�X
	std::vector<Node>				node_vec;	// �m�[�h

	float tile_count = 1;	// �e�N�X�`�����^�C�������鎞�̃^�C����(�����l��1)
private:
	std::weak_ptr<Transform3DComponent> transform_Wptr;

#ifdef _DEBUG
public:
	void DrawDebugGUI() override;
	void DrawDebugPrimitive()  override;
	/**
	 * �f�o�b�O�v���~�e�B�u�\���pImGui
	 */
	void DrawDebugPrimitiveGUI()  override;
	bool IsDebugPrimitive() override { return true; }   // DebugPrimitive�����݂��邩
private:
	const char* model_filename = nullptr;
	
	std::vector<AABBCorners> AABB_corners_vec;
#endif // _DEBUG
};

