#pragma once
#include <memory>

class Object;
class StateMachineComponent;

// ステートマシンで使用するステートの基底クラス
class State
{
public:
	// コンストラクタ
	State() = default;
	virtual ~State() {}
	// ステートに入った時のメソッド
	virtual void Start() = 0;
	// ステートで実行するメソッド
	virtual void Update(float elapsed_time) = 0;
	// ステートから出ていくときのメソッド
	virtual void End() = 0;

	// 所有者の設定
	void SetOwner(const std::shared_ptr<Object>& owner) { this->owner = owner; }
	// 所有者の取得
	std::shared_ptr<Object> GetOwner() const { return this->owner.lock(); }
protected:
	std::weak_ptr<Object> owner;								// 所有者のポインタ
	std::weak_ptr<StateMachineComponent> state_machine_Wptr;	// 自分が所属しているステートマシンのポインタ
};

class DefaultState : public State
{
public:
	// コンストラクタ
	DefaultState() {};
	~DefaultState() {}
	// ステートに入った時のメソッド
	void Start() override{};
	// ステートで実行するメソッド
	void Update(float elapsed_time) override {};
	// ステートから出ていくときのメソッド
	void End() override {};
};
