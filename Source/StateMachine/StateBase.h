#pragma once
#include <memory>
#include "System/MyHash.h"

class Object;

// ステートマシンで使用するステートの基底クラス
class State
{
public:
	// コンストラクタ
	State() = default;
	virtual ~State() {}
	// ステートに入った時のメソッド
	virtual void Enter() = 0;
	// ステートで実行するメソッド
	virtual void Execute(float elapsed_time) = 0;
	// ステートから出ていくときのメソッド
	virtual void Exit() = 0;

	void SetOwner(const std::shared_ptr<Object>& owner) { this->owner = owner; }
protected:
	std::weak_ptr<Object> owner;	// 所有者のポインタ
	MyHash state_name;
};

class Default_State : public State
{
public:
	// コンストラクタ
	Default_State() = default;
	~Default_State() {}
	// ステートに入った時のメソッド
	void Enter() override{};
	// ステートで実行するメソッド
	void Execute(float elapsed_time) override {};
	// ステートから出ていくときのメソッド
	void Exit() override {};
};
