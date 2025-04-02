#pragma once
#include "StateMachine\StateBase.h"

class TextNumberComponent;
class SpriteComponent;
class CharacterComponent;
class PlayerComponent;

// スコアUIの通常ステート
class ScoreUIDefaultState : public State
{
public:
	// コンストラクタ
	ScoreUIDefaultState();
	~ScoreUIDefaultState() {}
	// ステートに入った時のメソッド
	void Start() override {};
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
	EndTimerUIDefaultState();
	~EndTimerUIDefaultState() {}
	// ステートに入った時のメソッド
	void Start() override {};
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	std::weak_ptr<TextNumberComponent> text_number_Wptr;
};


// プレイヤー体力
class PlayerHPBarUIState : public State
{
public:
	// コンストラクタ
	PlayerHPBarUIState();
	~PlayerHPBarUIState() {}
	// ステートに入った時のメソッド
	void Start() override {};
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	float damage_flash_timer = 0.0f;
private:
	std::weak_ptr<SpriteComponent> sprite_Wptr;
	std::weak_ptr<CharacterComponent> player_health_Wptr;
};

// スペシャルポイントの通常ステート
class PlayerSpecialPointUIState : public State
{
public:
	// コンストラクタ
	PlayerSpecialPointUIState();
	~PlayerSpecialPointUIState() {}
	// ステートに入った時のメソッド
	void Start() override {};
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	float CalculateSpecialPointWidth();
private:
	std::weak_ptr<SpriteComponent> sprite_Wptr;
	std::weak_ptr<PlayerComponent> player_Wptr;
};

// スペシャルポイントフレームステート
class PlayerSpecialPointFrameUIState : public State
{
public:
	// コンストラクタ
	PlayerSpecialPointFrameUIState();
	~PlayerSpecialPointFrameUIState() {}
	// ステートに入った時のメソッド
	void Start() override {};
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	std::weak_ptr<SpriteComponent> sprite_Wptr;
	std::weak_ptr<PlayerComponent> player_Wptr;
};

// 回転攻撃説明UI
class SpinAttackDescriptionUIState : public State
{
public:
	// コンストラクタ
	SpinAttackDescriptionUIState();
	~SpinAttackDescriptionUIState() {}
	// ステートに入った時のメソッド
	void Start() override {};
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	std::weak_ptr<SpriteComponent> sprite_Wptr;
	std::weak_ptr<PlayerComponent> player_Wptr;
};
