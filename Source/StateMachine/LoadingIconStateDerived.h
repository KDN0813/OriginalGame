#pragma once
#include "StateMachine\StateBase.h"

class Transform2DComponent;

class LoadingIconAnimationState : public State
{
public:
	// コンストラクタ
	LoadingIconAnimationState();
	~LoadingIconAnimationState() {}
	// ステートに入った時のメソッド
	void Start() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override {};

private:
	float timer = 0.0f;
	const float ROTATION_SPEED = 10.0f;

private:
	std::weak_ptr<Transform2DComponent> transform_Wprt;
};