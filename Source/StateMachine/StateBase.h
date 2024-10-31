#pragma once
#include <memory>

class Object;

// ステートマシンで使用するステートの基底クラス
class State
{
public:
	// コンストラクタ
	State(const std::shared_ptr<Object>& owner) :owner(owner) {}
	virtual ~State() {}
	// ステートに入った時のメソッド
	virtual void Enter() = 0;
	// ステートで実行するメソッド
	virtual void Execute(float elapsed_time) = 0;
	// ステートから出ていくときのメソッド
	virtual void Exit() = 0;
protected:
	std::weak_ptr<Object> owner;	// 所有者のポインタ
};

class Default_State : public State
{
public:
	// コンストラクタ
	Default_State(const std::shared_ptr<Object>& owner) :State(owner) {}
	~Default_State() {}
	// ステートに入った時のメソッド
	void Enter() override{};
	// ステートで実行するメソッド
	void Execute(float elapsed_time) override {};
	// ステートから出ていくときのメソッド
	void Exit() override {};
};
