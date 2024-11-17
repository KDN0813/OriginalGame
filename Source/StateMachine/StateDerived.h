#pragma once
#include <memory>
#include "StateBase.h"

class ModelAnimationControlComponent;
class MovementComponent;
class PlayerComponent;
class CircleCollisionComponent;

class PlayerIdleState : public State
{
public:
	// コンストラクタ
	PlayerIdleState();
	~PlayerIdleState() {}
	// ステートに入った時のメソッド
	void Staet() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	State::ChangeState change_move_state;
	State::ChangeState change_attack_state;
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<MovementComponent> movement_Wpt;
};

class PlayerMoveState : public State
{
public:
	// コンストラクタ
	PlayerMoveState();
	~PlayerMoveState() {}
	// ステートに入った時のメソッド
	void Staet() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	State::ChangeState change_idle_state;
	State::ChangeState change_attack_state;
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<MovementComponent> movement_Wpt;
};

class PlayerAttackState : public State
{
public:
	// コンストラクタ
	PlayerAttackState();
	~PlayerAttackState() {}
	// ステートに入った時のメソッド
	void Staet() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override;
private:
	State::ChangeState change_idle_state;
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
	std::weak_ptr<CircleCollisionComponent> child_collision_Wprt;
};
