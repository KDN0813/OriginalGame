#include <memory>
#include <d3d11.h>
#include "Component/Component.h"
#include "Model/ModelCommonData.h"
#include "StateMachine/LegacyTransitionJudgement.h"

class ModelComponent;
class ModelResource;

// モデルクラスのアニメーションクラス
class ModelAnimationComponent : public Component
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
	// アニメーションの遷移情報
	struct AnimeTransitionInfo
	{
		AnimeIndex next_anime_index = -1;							// 次のアニメのインデックス
		std::unique_ptr<TransitionJudgementBase> judgement;	// 遷移判定
		float blend_time = 1.0f;									// ブレンド時間
	};
	// アニメーション状態
	struct AnimeState
	{
		std::string name = {};													// アニメーション名
		AnimeIndex anime_index = -1;											// アニメのインデックス
		bool loop = false;														// ループフラグ
		float transition_ready_time = -1.0f;									// 遷移準備が完了するまでの時間(0以下なら再生終了で準備完了)
		std::vector<std::unique_ptr<AnimeTransitionInfo>> transition_info_pool;	// 遷移するアニメーション情報
	};
public:
	ModelAnimationComponent(AnimationParam param ,const char* filename);

	// 開始関数
	void Start() override {};
	// リスタート処理
	void ReStart() override;      // パラメータの初期化
	// 更新関数
	void Update(float elapsed_time) override;
	// 名前取得
	const char* GetName()const override { return "ModelAnimationComponent"; };
	// 優先度
	const COMPONENT_PRIORITY GetPriority()const noexcept override { return COMPONENT_PRIORITY::MEDIUM; }

	// アニメーション更新処理
	void UpdateAnimation(float elapsed_time);
	// アニメーション再生
	void PlayAnimation(int index, bool loop, float blend_seconds = 0.2f);
	void PlayAnimation(const AnimeState& animation_info, float blend_seconds);
	// アニメーション再生中か
	bool IsPlayAnimation()const;
	// アニメーションの遷移準備が完了しているか
	// 遷移判定クラスで遷移準備を待つ設定の時に使用する
	bool IsTransitionReady();
	// 遷移判定のロジックを実行
	// `judgemenのshould_reverse` フラグがtrueなら、遷移判定結果を反転する
	bool PerformTransitionJudgement(TransitionJudgementBase* judgemen);

	// アニメーション状態の更新
	void UpdateAnimationState();
	// アニメーション状態の設定
	void SetAnimationState(AnimeIndex anime_index, bool loop, float transition_ready_time = -1.0f);
	// 遷移するアニメーションの追加
	void AddAnimationTransition(AnimeIndex anime_index, AnimeIndex transition_anime_index, std::unique_ptr<TransitionJudgementBase> judgement, float blend_time);

	// 各種データ取得
	float GetCurrentAnimationSeconds()const { return this->param.current_animation_seconds; }
private:
	std::vector<AnimeState>				anime_state_pool;	// アニメーション情報	
	int animation_size = 0;					// アニメーションの数
	AnimationParam param;
	AnimationParam default_param;

private:
	std::weak_ptr<ModelComponent> model_Wptr;
	std::weak_ptr<ModelResource> model_resource_Wptr;

#ifdef _DEBUG
public:
	void DrawDebugGUI() override;
	void DrawDetail();
private:
	std::vector<std::string> animation_name_pool;
	bool stop_anime_state_update = false;	// アニメーションステートの更新停止フラグ
	bool is_draw_deletail = false;
	int select_animation_index = 0;			// 詳細を表示するアニメーションのインデックス
#endif // _DEBUG
};
