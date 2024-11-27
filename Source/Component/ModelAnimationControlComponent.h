#pragma once
#include "Component.h"
#include "Model/ModelCommonData.h"

class ModelComponent;
class ModelResource;

// モデルクラスのアニメーションクラス
// 外部からの指示でアニメーションを制御する
class ModelAnimationControlComponent : public Component
{
public:
	struct AnimationParam
	{
		int current_animation_index = -1;		// 再生中のアニメーションのインデックス
		float current_animation_seconds = 0;	// 現在の再生時間
		float animation_blend_time = 0;			// アニメーションブレンドの経過時間
		float animation_blend_seconds = 0;		// アニメーションブレンドの時間
		bool animation_loop_flag = 0;			// ループフラグ
		bool animation_end_flag = 0;			// 終了フラグ
		bool dummy[2]{};
	};
public:
	ModelAnimationControlComponent(const char* filename);

	// 開始関数
	void Start() override;
	// リスタート処理
	void ReStart() override;      // パラメータの初期化
	// 更新関数
	void Update(float elapsed_time) override;
	// 名前取得
	const char* GetName()const override { return "ModelAnimationControlComponent"; };
	// 優先度
	const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::MEDIUM; }

	// アニメーション更新処理
	void UpdateAnimation(float elapsed_time);
	// アニメーション再生
	void PlayAnimation(int index, bool loop, float blend_seconds = 0.2f);
	// アニメーション再生中か
	bool IsPlayAnimation()const;

	// 各種データ取得
	float GetCurrentAnimationSeconds()const { return this->param.current_animation_seconds; }
private:
	AnimationParam param;

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

