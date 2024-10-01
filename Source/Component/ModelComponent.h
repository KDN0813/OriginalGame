#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <vector>
#include <string>
#include <memory>
#include "Component.h"
#include "Model/AnimeTransitionJudgement.h"
#include "Model/ModelHelpers.h"

class ModelResource;

class Transform3DComponent;

// アニメーションの遷移情報
struct AnimeTransitionInfo
{
	AnimeIndex next_anime_index = -1;							// 次のアニメのインデックス
	std::unique_ptr<AnimeTransitionJudgementBase> judgement;	// 遷移判定
	float blend_time = 1.0f;									// ブレンド時間
};

// アニメーション状態
struct AnimeState
{
	std::string name = {};									// アニメーション名
	AnimeIndex anime_index = -1;							// アニメのインデックス
	bool loop = false;										// ループフラグ
	std::vector<std::unique_ptr<AnimeTransitionInfo>> transition_info_pool;	// 遷移するアニメーション情報
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
	void Start();
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
	void PlayAnimation(const AnimeState& animation_info, float blend_seconds);
	// アニメーション再生中か
	bool IsPlayAnimation()const;
	// ノード検索
	ModelComponent::Node* FindNode(const char* name);

	// アニメーション状態の更新
	void UpdateAnimationState();
	// アニメーション状態の設定
	void SetAnimationState(AnimeIndex anime_index, bool loop);
	// 遷移するアニメーションの追加
	void AddAnimationTransition(AnimeIndex anime_index, AnimeIndex transition_anime_index, std::unique_ptr<AnimeTransitionJudgementBase> judgement, float blend_time);

	// 各種データ取得
	const std::vector<Node>& GetNodes() const { return node_vec; }
	std::vector<Node>& GetNodes() { return node_vec; }
	const ModelResource* GetResource() const { return resource.get(); }
	float GetCurrentAnimationSeconds()const { return current_animation_seconds; }

private:
	std::vector<AnimeState>				anime_state_pool;	// アニメーション情報	

	std::shared_ptr<ModelResource>	resource;	// モデルリソース
	std::vector<Node>				node_vec;	// ノード

	int current_animation_index  = -1;		// 再生中のアニメーションのインデックス
	float current_animation_seconds = 0;	// 現在の再生時間
	float animation_blend_time = 0;			// アニメーションブレンドの経過時間
	float animation_blend_seconds = 0;		// アニメーションブレンドの時間
	bool animation_loo_flag = 0;			// ループフラグ
	bool animation_end_flag = 0;			// 終了フラグ
	bool dummy[2]{};
private:
	std::weak_ptr<Transform3DComponent> transform_Wptr;

#ifdef _DEBUG
public:
	void DrawDebugGUI() override;

#endif // _DEBUG
};

