#pragma once
#include "StateMachine\StateBase.h"

class TextNumberComponent;

// スコアUIの通常ステート
class ScoreUIDefaultState : public State
{
public:
	// コンストラクタ
	ScoreUIDefaultState() :State("ScoreUIDefaultState") {};
	~ScoreUIDefaultState() {}
	// ステートに入った時のメソッド
	void Staet() override {};
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	std::weak_ptr<TextNumberComponent> text_number_Wptr;
};

// 終了タイマーの通常ステート
class EndTimerUIDefaultState : public State
{
public:
	// コンストラクタ
	EndTimerUIDefaultState() :State("EndTimerUIDefaultState") {};
	~EndTimerUIDefaultState() {}
	// ステートに入った時のメソッド
	void Staet() override {};
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	std::weak_ptr<TextNumberComponent> text_number_Wptr;
};
