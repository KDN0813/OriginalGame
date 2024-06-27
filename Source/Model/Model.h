#pragma once

#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "Model/ModelResource.h"

// TODO (06/25)開発指針が定まり次第改善
class Model
{
public:
	Model(const char* filename);
	~Model() {}

	struct Node
	{
		Node() = default;
		const char* name;
		Node* parent;
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;
		DirectX::XMFLOAT4X4	localTransform;
		DirectX::XMFLOAT4X4	worldTransform;

		std::vector<Node*>	children;
	};

	// 各種データ取得
	const std::vector<Node>& GetNodes() const { return nodes; }
	std::vector<Node>& GetNodes() { return nodes; }
	const ModelResource* GetResource() const { return resource.get(); }
	float GetCurrentAnimationSeconds()const { return current_animation_seconds; }

	// 行列計算
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);

	// アニメーション更新処理
	void UpdateAnimation(float elapsedTIme);

	// アニメーション再生
	void PlayAnimation(int index,bool loop,float blendSeconds = 0.2f);

	// アニメーション再生中か
	bool IsPlayAnimation()const;

	// ノード検索
	Model::Node* FindNode(const char* name);

private:
	std::shared_ptr<ModelResource>	resource;
	std::vector<Node>				nodes;

	int current_animation_index;
	float current_animation_seconds;
	float animation_blend_time;
	float animation_blend_seconds;
	bool animation_loo_flag;
	bool animation_end_flag;
	bool dummy[3]{};
};
