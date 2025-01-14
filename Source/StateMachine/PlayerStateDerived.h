#pragma once
#include <memory>
#include "StateBase.h"

class ModelAnimationControlComponent;
class MovementComponent;
class PlayerComponent;
class CircleCollisionComponent;
class CharacterComponent;

class PlayerIdleState : public State
{
public:
	static const MyHash STATE_NAME;
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
	State::ChangeState change_damage_state;
	State::ChangeState change_spin_attack_state;
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<MovementComponent> movement_Wpt;
	std::weak_ptr<CharacterComponent> character_Wptr;
};

class PlayerMoveState : public State
{
public:
	static const MyHash STATE_NAME;
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
	State::ChangeState change_damage_state;
	State::ChangeState change_attack_state;
	State::ChangeState change_spin_attack_state;
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<MovementComponent> movement_Wpt;
	std::weak_ptr<CharacterComponent> character_Wptr;
};

class PlayerAttackState : public State
{
public:
	static const MyHash STATE_NAME;
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
	std::weak_ptr<CharacterComponent> character_Wptr;
};

// 回転攻撃
class PlayerSpinAttackState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// コンストラクタ
	PlayerSpinAttackState();
	~PlayerSpinAttackState() {}
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
	std::weak_ptr<CharacterComponent> character_Wptr;
};

// ダメージステート
class PlayerDamageState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// コンストラクタ
	PlayerDamageState();
	~PlayerDamageState() {}
	// ステートに入った時のメソッド
	void Staet() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override;
private:
	State::ChangeState change_idle_state;
	State::ChangeState change_move_state;
	State::ChangeState change_attack_state;
	State::ChangeState change_spin_attack_state;
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
};