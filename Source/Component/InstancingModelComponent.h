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
class AnimatedInstancedModelComponent : public Component
{
public:
	// アニメーションの遷移情報
	struct AnimeTransitionInfo
	{
		AnimeIndex next_anime_index = -1;							// 次のアニメのインデックス
		std::unique_ptr<AnimeTransitionJudgementBase> judgement;	// 遷移判定
	};
	// アニメーション状態
	struct AnimeState
	{
		std::string name = {};													// アニメーション名
		AnimeIndex anime_index = -1;											// アニメのインデックス
		bool loop = false;														// ループフラグ
		float transition_ready_time = -1;										// 遷移準備が完了するまでの時間(0以下なら再生終了で準備完了)
		std::vector<std::unique_ptr<AnimeTransitionInfo>> transition_info_pool;	// 遷移するアニメーション情報
	};
public:
	AnimatedInstancedModelComponent(ID3D11Device* device, const char* filename);

    void Update(float elapsed_time) override;

	const char* GetName()const override { return "AnimatedInstancedModelComponent"; }
	// 優先度
	const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::LOW; }
	
	void PlayAnimation(int animeIndex, bool loop = true);
	void PlayAnimation(const AnimeState& animation_info);

	void UpdateAnimation(float elapsed_time);
	// アニメーション状態の更新
	void UpdateAnimationState();
	// アニメーション状態の設定
	void SetAnimationState(AnimeIndex anime_index, bool loop, float transition_ready_time = -1.0f);
	// 遷移するアニメーションの追加
	void AddAnimationTransition(AnimeIndex anime_index, AnimeIndex transition_anime_index, std::unique_ptr<AnimeTransitionJudgementBase> judgement);

	// アニメーションの遷移準備が完了しているか
	// 遷移判定クラスで遷移準備を待つ設定の時に使用する
	bool IsTransitionReady();
	// 遷移判定のロジックを実行
	// `judgemenのshould_reverse` フラグがtrueなら、遷移判定結果を反転する
	bool PerformTransitionJudgement(AnimeTransitionJudgementBase* judgemen);

	// 各取得・設定関数
	InstancingModelResource* GetInstancingModelResource() { return this->instancing_model_resource.get(); }
	ModelResource* GetModelResource() { return this->model_resource.get(); }
	UINT GetAnimeFrame();
	float GetCurrentAnimationSeconds() const { return this->current_animation_seconds; }
	UINT GetAnimationStartOffset();
	int GetModelId();
private:
	std::shared_ptr<InstancingModelResource> instancing_model_resource;
	std::shared_ptr<ModelResource> model_resource;

	std::vector<AnimeState>	anime_state_pool;	// アニメーション情報

	float current_animation_seconds = 0.0f;		// 現在の再生時間
	UINT anime_index = 0;
	int anime_speed = 1;
	bool anime_loop = false;
	bool anime_play = false;					// アニメーションが再生中であるか

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

