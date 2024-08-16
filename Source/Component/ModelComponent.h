#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Component.h"

class ModelResource;

class Transform3DComponent;

class ModelComponent : public Component
{
public:
	struct Node
	{
		const char* name = {};
		Node* parent = {};
		DirectX::XMFLOAT3	scale = {};
		DirectX::XMFLOAT4	rotate = {};
		DirectX::XMFLOAT3	translate = {};
		DirectX::XMFLOAT4X4	local_transform = {};
		DirectX::XMFLOAT4X4	world_transform = {};
		std::vector<Node*>	children;
	};

public:
	ModelComponent(const char* filename);

	// 更新関数
	void Update(float elapsed_time);
	// 名前取得
	const char* GetName()const { return "ModelComponent"; };

	// 行列計算
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);
	// アニメーション更新処理
	void UpdateAnimation(float elapsed_time);
	// アニメーション再生
	void PlayAnimation(int index, bool loop, float blend_seconds = 0.2f);
	// アニメーション再生中か
	bool IsPlayAnimation()const;
	// ノード検索
	ModelComponent::Node* FindNode(const char* name);

	// 各種データ取得
	const std::vector<Node>& GetNodes() const { return node_vec; }
	std::vector<Node>& GetNodes() { return node_vec; }
	const ModelResource* GetResource() const { return resource.get(); }
	float GetCurrentAnimationSeconds()const { return current_animation_seconds; }

private:
	std::shared_ptr<ModelResource>	resource;
	std::vector<Node>				node_vec;

	int current_animation_index  = -1;
	float current_animation_seconds = 0;
	float animation_blend_time = 0;
	float animation_blend_seconds = 0;
	bool animation_loo_flag = 0;
	bool animation_end_flag = 0;
	bool dummy[2]{};
private:
	std::weak_ptr<Transform3DComponent> transform_Wptr;

#ifdef _DEBUG
public:
	void DrawDebugGUI() override;

#endif // _DEBUG
};

