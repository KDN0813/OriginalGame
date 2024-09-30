#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <vector>
#include "Component.h"

class ModelResource;

class Transform3DComponent;

using AnimeIndex = size_t;

// アニメーション遷移判定
class AnimationTransitionJudgementBase
{
public:
	AnimationTransitionJudgementBase(){}
	virtual ~AnimationTransitionJudgementBase() {}

	virtual bool Judgement() = 0;
};

// 遷移するアニメーション情報
struct AnimationTransitionState
{
	AnimeIndex next_anime_index = -1;							// 次のアニメのインデックス
	AnimationTransitionJudgementBase* judgement = nullptr;		// 遷移判定
	float blend_time = 1.0f;									// ブレンド時間
};

// アニメーション情報
struct AnimationInfo
{
	bool loop = false;
	AnimeIndex anime_index = -1;
	std::vector<AnimationTransitionState*> next_animation_vec;
};

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
	ModelComponent(ID3D11Device* device, const char* filename);

	// 開始関数
	void Initialize();
	// 更新関数
	void Update(float elapsed_time);
	// 名前取得
	const char* GetName()const { return "ModelComponent"; };
	// 優先度
	const unsigned int GetPriority()const noexcept override { return 2; }

	// 行列計算
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);
	// アニメーション更新処理
	void UpdateAnimation(float elapsed_time);
	// アニメーション再生
	void PlayAnimation(int index, bool loop, float blend_seconds = 0.2f);
	void PlayAnimation(AnimationInfo* animation_info, float blend_seconds);
	// アニメーション再生中か
	bool IsPlayAnimation()const;
	// ノード検索
	ModelComponent::Node* FindNode(const char* name);

	// アニメーションの更新
	void AnimationStateUpdate();

	// 各種データ取得
	const std::vector<Node>& GetNodes() const { return node_vec; }
	std::vector<Node>& GetNodes() { return node_vec; }
	const ModelResource* GetResource() const { return resource.get(); }
	float GetCurrentAnimationSeconds()const { return current_animation_seconds; }

private:
	std::vector<AnimationInfo*>				animation_info_vec;	// アニメーション情報	

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

