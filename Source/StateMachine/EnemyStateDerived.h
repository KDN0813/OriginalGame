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
	std::weak_ptr<InstancedModelWithAnimationComponent> animation_Wprt;
	std::weak_ptr<CharacterComponent> character_Wptr;
	std::weak_ptr<EnemyComponent> enemy_Wptr;
};

// 徘徊ステート
class EnemyWanderState : public State
{
public:
	// コンストラクタ
	EnemyWanderState();
	~EnemyWanderState() {}
	// ステートに入った時のメソッド
	void Start() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override;
private:
	std::weak_ptr<InstancedModelWithAnimationComponent> animation_Wprt;
	std::weak_ptr<Transform3DComponent> transform_Wprt;
	std::weak_ptr<CharacterComponent> character_Wptr;
	std::weak_ptr<EnemyComponent> enemy_Wptr;
};

// 接近ステート
class EnemyChaseState : public State
{
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
	std::weak_ptr<InstancedModelWithAnimationComponent> animation_Wprt;
	std::weak_ptr<EnemyComponent> enemy_Wptr;
	std::weak_ptr<CharacterComponent> character_Wptr;
};

// 攻撃ステート
class EnemyAttackState : public State
{
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
	std::weak_ptr<InstancedModelWithAnimationComponent> animation_Wprt;
	std::weak_ptr<CircleCollisionComponent> child_collision_Wprt;
	std::weak_ptr<EnemyComponent> enemy_Wptr;
	std::weak_ptr<CharacterComponent> character_Wptr;
};

// ダメージステート
class EnemyDamageState : public State
{
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
	std::weak_ptr<InstancedModelWithAnimationComponent> animation_Wprt;
	std::weak_ptr<CharacterComponent> character_Wptr;
};

// 死亡ステート
class EnemyDeadState : public State
{
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
	float remove_timer = {};
private:
	std::weak_ptr<InstancedModelWithAnimationComponent> animation_Wprt;
	std::weak_ptr<EnemyComponent> enemy_Wptr;
};

// 死亡待機ステート
class EnemyDeadIdleState : public State
{
public:
	// コンストラクタ
	EnemyDeadIdleState();
	~EnemyDeadIdleState() {}
	// ステートに入った時のメソッド
	void Start() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	float remove_timer = {};
private:
	std::weak_ptr<InstancedModelWithAnimationComponent> animation_Wprt;
};

// 生成ステート
class EnemySpawnState : public State
{
public:
	const float IDLE_TIME = 1.0f;
public:
	// コンストラクタ
	EnemySpawnState();
	~EnemySpawnState() {}
	// ステートに入った時のメソッド
	void Start() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override;
private:
	float idle_timer = 0.0f;	// 待機時間
private:
	std::weak_ptr<InstancedModelWithAnimationComponent> animation_Wprt;
	std::weak_ptr<EnemyComponent> enemy_Wptr;
	std::weak_ptr<CharacterComponent> character_Wptr;
};
