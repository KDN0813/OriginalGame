#pragma once
#include "Component/Component.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
#include "Model/ModelHelpers.h"

class InstancingModelResource;
class ModelResource;

class Transform3DComponent;

class InstancingModelComponent : public Component
{
public:
public:
	InstancingModelComponent(ID3D11Device* device, const char* filename);

    void Update(float elapsed_time) override;

	const char* GetName()const override { return "InstancingModelComponent"; }
	// 優先度
	const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::LOW; }
	
	void PlayAnimation(int animeIndex, bool loop = true);
	void PlayAnimation(const AnimeState& animation_info);

	void UpdateAnimation();
	// アニメーション状態の更新
	void UpdateAnimationState();
	// アニメーション状態の設定
	void SetAnimationState(AnimeIndex anime_index, bool loop, float transition_ready_time = -1.0f);
	// 遷移するアニメーションの追加
	void AddAnimationTransition(AnimeIndex anime_index, AnimeIndex transition_anime_index, std::unique_ptr<AnimeTransitionJudgementBase> judgement, float blend_time);

	// 各取得・設定関数
	InstancingModelResource* GetInstancingModelResource() { return this->instancing_model_resource.get(); }
	ModelResource* GetModelResource() { return this->model_resource.get(); }
	const UINT GetAnimeFrame() { return this->anime_frame; }
	const UINT GetAnimationStartOffset();
	const int GetModelId();
private:
	std::shared_ptr<InstancingModelResource> instancing_model_resource;
	std::shared_ptr<ModelResource> model_resource;

	std::vector<AnimeState>	anime_state_pool;	// アニメーション情報

	UINT anime_frame = 0;
	UINT anime_index = 0;
	int anime_speed = 1;
	bool anime_loop = false;
	bool anime_play = false;

private:
	std::weak_ptr<Transform3DComponent> transform_Wptr;

#ifdef _DEBUG
public:
	void DrawDebugGUI()override;
	void DrawDebugAnimationGUI();
	void DrawDetail();

private:
	std::vector<std::string> animation_name_pool;
	bool stop_anime_state_update = false;
	bool is_draw_deletail = false;
	int select_animation_index = 0;			// 詳細を表示するアニメーションのインデックス
	const char* model_filename;
#endif // _DEBUG
};

