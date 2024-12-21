#pragma once
#include "Component/InstancedModelWithAnimationComponent.h"

// アニメーション付インスタンスモデル
// 内部でステートを管理し、独立してアニメーション遷移を行う
class InstancedModelWithStateAnimationComponent : public InstancedModelWithAnimationComponent
{
public:
	// アニメーションの遷移情報
	struct AnimeTransitionInfo
	{
		AnimeIndex next_anime_index = -1;							// 次のアニメのインデックス
		std::unique_ptr<TransitionJudgementBase> judgement;	// 遷移判定
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
	InstancedModelWithStateAnimationComponent(InstancedModelParam param, const char* filename);

	// リスタート処理
	void ReStart() override;      // パラメータの初期化
    void Update(float elapsed_time) override;

	const char* GetName()const override { return "AnimatedInstancedModelComponent"; }
	// 優先度
	const PRIORITY GetPriority()const noexcept override { return PRIORITY::LOW; }
	
	void PlayAnimation(const AnimeState& animation_info);

	// アニメーション状態の更新
	void UpdateAnimationState();
	// アニメーション状態の設定
	void SetAnimationState(AnimeIndex anime_index, bool loop, float transition_ready_time = -1.0f);
	// 遷移するアニメーションの追加
	void AddAnimationTransition(AnimeIndex anime_index, AnimeIndex transition_anime_index, std::unique_ptr<TransitionJudgementBase> judgement);

	// アニメーションの遷移準備が完了しているか
	// 遷移判定クラスで遷移準備を待つ設定の時に使用する
	bool IsTransitionReady();
	// 遷移判定のロジックを実行
	// `judgemenのshould_reverse` フラグがtrueなら、遷移判定結果を反転する
	bool PerformTransitionJudgement(TransitionJudgementBase* judgemen);

	// 各取得・設定関数
private:
	std::vector<AnimeState>	anime_state_pool;	// アニメーション情報

#ifdef _DEBUG
public:
	void DrawDebugGUI()override;
	void DrawDebugAnimationGUI();
	void DrawDetail();

private:
	bool stop_anime_state_update = false;
	bool is_draw_deletail = false;
	int select_animation_index = 0;			// 詳細を表示するアニメーションのインデックス
#endif // _DEBUG
};

