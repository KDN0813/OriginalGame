#pragma once
#include "Component.h"
#include "Model/ModelCommonData.h"
#include "Model\ModelResource.h"

class ModelComponent;

// モデルクラスのアニメーションクラス
// パーツ毎に別のアニメーションを再生できる
class ModelPartAnimationControlComponent : public Component
{
public:
	struct InitAnimeParam
	{
		int init_anime_index = -1;				// 最初に再生するアニメーションインデックス
		bool init_anime_loop = false;			// 最初に再生するアニメーションのループフラグ
	};
	struct AnimationParam
	{
		int current_animation_index = -1;		// 再生中のアニメーションのインデックス
		float current_animation_seconds = 0.0f;	// 現在の再生時間
		float animation_blend_time = 0.0f;			// アニメーションブレンドの経過時間
		float animation_blend_seconds = 0.0f;		// アニメーションブレンドの時間
		bool animation_loop_flag = 0;			// ループフラグ
		bool animation_end_flag = 0;			// 終了フラグ
		bool dummy[2]{};
	};

	struct PartsParam
	{
		AnimationParam anime_param;							// アニメーション情報
		std::vector<ModelResource::NodeId> node_index_vec;	// 上記の情報で更新するノードのインデックス
	};
public:
	ModelPartAnimationControlComponent(InitAnimeParam init_param);

	// 開始関数
	void Start() override;
	// リスタート処理
	void ReStart() override;      // パラメータの初期化
	// 更新関数
	void Update(float elapsed_time) override;
	// 名前取得
	const char* GetName()const override { return "ModelPartAnimationControlComponent"; };
	// 優先度
	const PRIORITY GetPriority()const noexcept override { return PRIORITY::MEDIUM; }

	/**
	 * アニメーション更新処理
	 * 
	 * \param parts	更新するパーツの参照
	 * \param elapsed_time 経過時間
	 */
	void UpdateAnimation(PartsParam& parts, float elapsed_time);
	// アニメーション再生
	void PlayAnimation(int index, bool loop, float blend_seconds = 0.2f);
	// アニメーション再生中か
	bool IsPlayAnimation()const;

private:
	InitAnimeParam init_param;
	
	PartsParam main_parts;

private:
	std::weak_ptr<ModelComponent> model_Wptr;
	size_t animation_size = 0;

#ifdef _DEBUG
public:
	void DrawDebugGUI() override;
private:
	std::vector<std::string> animation_name_pool;
#endif // _DEBUG
};

