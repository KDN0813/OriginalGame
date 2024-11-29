#pragma once
#include "StateMachine\StateBase.h"

class SpriteComponent;

class LoadingIconAnimationState : public State
{
public:
	// コンストラクタ
	LoadingIconAnimationState();
	~LoadingIconAnimationState() {}
	// ステートに入った時のメソッド
	void Staet() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override {};

private:
	float timer = 0.0f;

private:
	std::weak_ptr<SpriteComponent> sprite_Wprt;
};