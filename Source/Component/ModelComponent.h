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
	ModelComponent(const char* filename);

	// 開始関数
	void Start() override;
	// リスタート処理
	void ReStart() override {};
	// 更新関数
	void Update(float elapsed_time) override;
	// 名前取得
	const char* GetName()const override { return "ModelComponent"; };
	// 優先度
	const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::LOW; }

	// 行列計算
	void UpdateTransform(MYMATRIX Transform);

	// ノード検索
	ModelComponent::Node* FindNode(const char* name);

	// 各種データ取得
	const std::vector<Node>& GetNodes() const { return node_vec; }
	std::vector<Node>& GetNodes() { return node_vec; }
	const ModelResource* GetResource() const { return resource.get(); }
	std::shared_ptr<ModelResource>& GetResourceSptr() { return resource; }
	float GetTileCount() const { return this->tile_count; }
	void SetTileCount(float tile_count) { this->tile_count = tile_count; }

private:
	std::shared_ptr<ModelResource>	resource;	// モデルリソース
	std::vector<Node>				node_vec;	// ノード

	// TODO 地形モデルを実装したら削除する
	float tile_count = 1;	// テクスチャをタイル化する時のタイル数(初期値は1)
private:
	std::weak_ptr<Transform3DComponent> transform_Wptr;

#ifdef _DEBUG
public:
	void DrawDebugGUI() override;
private:
	const char* model_filename;
#endif // _DEBUG
};

