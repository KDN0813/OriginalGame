#pragma once
#include <vector>
#include <string>
#include "Model/AnimeTransitionJudgement.h"

using AnimeIndex = size_t;

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
	std::string name = {};													// アニメーション名
	AnimeIndex anime_index = -1;											// アニメのインデックス
	bool loop = false;														// ループフラグ
	float transition_ready_time = -1.0f;									// 遷移準備が完了するまでの時間(0以下なら再生終了で準備完了)
	std::vector<std::unique_ptr<AnimeTransitionInfo>> transition_info_pool;	// 遷移するアニメーション情報
};
