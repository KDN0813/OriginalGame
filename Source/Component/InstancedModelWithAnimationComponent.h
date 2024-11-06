#pragma once
#include "Component/Component.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
#include "Model/ModelCommonData.h"

class InstancingModelResource;
class ModelResource;

class Transform3DComponent;

// アニメーション付インスタンスモデル
class InstancedModelWithAnimationComponent : public Component
{
public:
	struct InstancedModelParam
	{
		UINT anime_index = 0;
		float current_animation_seconds = 0.0f;		// 現在の再生時間
		int anime_speed = 1;
		bool anime_loop = false;
		bool anime_play = false;					// アニメーションが再生中であるか
	};
public:
	// 再生するアニメーションのパラメータ
	struct PlayAnimeParam
	{
		UINT anime_index = 0;
		bool loop = false;
		float anime_speed = 1.0f;
	};
public:
	InstancedModelWithAnimationComponent(InstancedModelParam param, const char* filename);

	// リスタート処理
	void ReStart() override;      // パラメータの初期化
	void Update(float elapsed_time) override;

	const char* GetName()const override { return "AnimatedInstancedModelComponent"; }
	// 優先度
	const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::LOW; }

	void PlayAnimation(int animeIndex, bool loop = true);
	void PlayAnimation(const PlayAnimeParam& play_anime_param);

	void UpdateAnimation(float elapsed_time);

	// 各取得・設定関数
	InstancingModelResource* GetInstancingModelResource() { return this->instancing_model_resource.get(); }
	ModelResource* GetModelResource() { return this->model_resource.get(); }
	UINT GetAnimeFrame();		// 現在の再生フレーム数を取得
	float GetCurrentAnimationSeconds() const { return this->param.current_animation_seconds; }
	UINT GetAnimationStartOffset();	// 現在再生しているアニメーションのオフセット値を取得
	int GetModelId();
protected:
	std::shared_ptr<InstancingModelResource> instancing_model_resource;
	std::shared_ptr<ModelResource> model_resource;

	InstancedModelParam param;
	InstancedModelParam default_param;

protected:
	std::weak_ptr<Transform3DComponent> transform_Wptr;

#ifdef _DEBUG
public:
	void DrawDebugGUI()override;
	void DrawDebugAnimationGUI();

protected:
	std::vector<std::string> animation_name_pool;
	const char* model_filename;
#endif // _DEBUG
};

