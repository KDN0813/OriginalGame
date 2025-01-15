#pragma once
#include "StateMachine\StateBase.h"

class ModelAnimationControlComponent;
class MovementComponent;
class CharacterComponent;

// 待機ステート
class EnemyIdleState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// コンストラクタ
	EnemyIdleState();
	~EnemyIdleState() {}
	// ステートに入った時のメソッド
	void Start() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	State::ChangeState change_wandering_state;
	State::ChangeState change_chase_state;
	State::ChangeState change_attack_state;
	State::ChangeState change_damage_state;
	State::ChangeState change_deth_state;
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<MovementComponent> movement_Wpt;
	std::weak_ptr<CharacterComponent> character_Wptr;
};

// 徘徊ステート
class EnemyWanderingState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// コンストラクタ
	EnemyWanderingState();
	~EnemyWanderingState() {}
	// ステートに入った時のメソッド
	void Start() override {};
	// ステートで実行するメソッド
	void Update(float elapsed_time) override {};
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	State::ChangeState change_idle_state;
	State::ChangeState change_chase_state;
	State::ChangeState change_attack_state;
	State::ChangeState change_damage_state;
	State::ChangeState change_deth_state;
};

// 接近ステート
class EnemyChaseState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// コンストラクタ
	EnemyChaseState();
	~EnemyChaseState() {}
	// ステートに入った時のメソッド
	void Start() override {};
	// ステートで実行するメソッド
	void Update(float elapsed_time) override {};
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	State::ChangeState change_idle_state;
	State::ChangeState change_chase_state;
	State::ChangeState change_attack_state;
};

// 攻撃ステート
class EnemyAttackState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// コンストラクタ
	EnemyAttackState();
	~EnemyAttackState() {}
	// ステートに入った時のメソッド
	void Start() override {};
	// ステートで実行するメソッド
	void Update(float elapsed_time) override {};
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	State::ChangeState change_idle_state;
};

// ダメージステート
class EnemyDamageState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// コンストラクタ
	EnemyDamageState();
	~EnemyDamageState() {}
	// ステートに入った時のメソッド
	void Start() override {};
	// ステートで実行するメソッド
	void Update(float elapsed_time) override {};
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	State::ChangeState change_idle_state;
};

// 死亡ステート
class EnemyDethState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// コンストラクタ
	EnemyDethState();
	~EnemyDethState() {}
	// ステートに入った時のメソッド
	void Start() override {};
	// ステートで実行するメソッド
	void Update(float elapsed_time) override {};
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	State::ChangeState change_deth_idle_state;
};

// 死亡待機ステート
class EnemyDethIdleState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// コンストラクタ
	EnemyDethIdleState();
	~EnemyDethIdleState() {}
	// ステートに入った時のメソッド
	void Start() override {};
	// ステートで実行するメソッド
	void Update(float elapsed_time) override {};
	// ステートから出ていくときのメソッド
	void End() override {};
private:
};