#pragma once
#include "StateBase.h"

class IdleState : public State
{
public:
	// コンストラクタ
	IdleState() :State("IdleState") {};
	~IdleState() {}
	// ステートに入った時のメソッド
	void Staet() override {};
	// ステートで実行するメソッド
	void Update(float elapsed_time) override {};
	// ステートから出ていくときのメソッド
	void End() override {};
};
