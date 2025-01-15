#pragma once
#include <DirectXMath.h>
#include <memory>
#include "StateMachine\StateBase.h"

class InstancedModelWithAnimationComponent;
class CharacterComponent;
class EnemyComponent;
class Transform3DComponent;
class CircleCollisionComponent;

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
	// 待機時間を設定する
	void SetRandomIdleTime();
private:
	float idle_timer = 0.0f;	// 待機時間
private:
	State::ChangeState change_wandering_state;
	State::ChangeState change_chase_state;
	State::ChangeState change_attack_state;
	State::ChangeState change_damage_state;
	State::ChangeState change_deth_state;
private:
	std::weak_ptr<InstancedModelWithAnimationComponent> animation_Wprt;
	std::weak_ptr<CharacterComponent> character_Wptr;
	std::weak_ptr<EnemyComponent> enemy_Wptr;
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
	void Start() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override;
private:
	State::ChangeState change_idle_state;
	State::ChangeState change_chase_state;
	State::ChangeState change_attack_state;
	State::ChangeState change_damage_state;
	State::ChangeState change_deth_state;
private:
	std::weak_ptr<InstancedModelWithAnimationComponent> animation_Wprt;
	std::weak_ptr<Transform3DComponent> transform_Wprt;
	std::weak_ptr<EnemyComponent> enemy_Wptr;
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
	void Start() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override;
private:
	State::ChangeState change_idle_state;
	State::ChangeState change_chase_state;
	State::ChangeState change_attack_state;
private:
	std::weak_ptr<InstancedModelWithAnimationComponent> animation_Wprt;
	std::weak_ptr<EnemyComponent> enemy_Wptr;
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
	void Start() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override;
private:
	State::ChangeState change_idle_state;
	State::ChangeState change_chase_state;
private:
	std::weak_ptr<InstancedModelWithAnimationComponent> animation_Wprt;
	std::weak_ptr<CircleCollisionComponent> child_collision_Wprt;
	std::weak_ptr<EnemyComponent> enemy_Wptr;
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
	void Start() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	State::ChangeState change_idle_state;
private:
	std::weak_ptr<InstancedModelWithAnimationComponent> animation_Wprt;
};

// 死亡ステート
class EnemyDeadState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// コンストラクタ
	EnemyDeadState();
	~EnemyDeadState() {}
	// ステートに入った時のメソッド
	void Start() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	State::ChangeState change_deth_idle_state;
private:
	std::weak_ptr<InstancedModelWithAnimationComponent> animation_Wprt;
};

// 死亡待機ステート
class EnemyDeadIdleState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// コンストラクタ
	EnemyDeadIdleState();
	~EnemyDeadIdleState() {}
	// ステートに入った時のメソッド
	void Start() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override {};
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	std::weak_ptr<InstancedModelWithAnimationComponent> animation_Wprt;
};