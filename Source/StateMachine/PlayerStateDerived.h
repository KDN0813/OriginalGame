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
	void Start() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	State::ChangeState change_move_state;
	State::ChangeState change_attack_state;
	State::ChangeState change_damage_state;
	State::ChangeState change_spin_attack_state;
	State::ChangeState change_dead_state;
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<MovementComponent> movement_Wpt;
	std::weak_ptr<CharacterComponent> character_Wptr;
	std::weak_ptr<PlayerComponent> player_Wptr;
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
	void Start() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	State::ChangeState change_idle_state;
	State::ChangeState change_damage_state;
	State::ChangeState change_attack_state;
	State::ChangeState change_spin_attack_state;
	State::ChangeState change_dead_state;
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<MovementComponent> movement_Wpt;
	std::weak_ptr<CharacterComponent> character_Wptr;
	std::weak_ptr<PlayerComponent> player_Wptr;
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
	void Start() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override;
private:
	State::ChangeState change_dead_state;
	State::ChangeState change_attack_combo2_state;
	State::ChangeState change_attack_hold_state;
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
	std::weak_ptr<CircleCollisionComponent> child_collision_Wprt;
	std::weak_ptr<CharacterComponent> character_Wptr;
};

class PlayerAttackLCombo2State : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// コンストラクタ
	PlayerAttackLCombo2State();
	~PlayerAttackLCombo2State() {}
	// ステートに入った時のメソッド
	void Start() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override;
private:
	State::ChangeState change_dead_state;
	State::ChangeState change_attack_state;
	State::ChangeState change_attack_hold_state;
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
	std::weak_ptr<CircleCollisionComponent> child_collision_Wprt;
	std::weak_ptr<CharacterComponent> character_Wptr;
};

// 回転攻撃開始ステート
class PlayerSpinAttackStartState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// コンストラクタ
	PlayerSpinAttackStartState();
	~PlayerSpinAttackStartState() {}
	// ステートに入った時のメソッド
	void Start() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	State::ChangeState change_spin_attack_state;
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
	std::weak_ptr<CircleCollisionComponent> child_collision_Wprt;
	std::weak_ptr<CharacterComponent> character_Wptr;
};

// 回転攻撃(ループ)
class PlayerSpinAttackSpinLoopState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// コンストラクタ
	PlayerSpinAttackSpinLoopState();
	~PlayerSpinAttackSpinLoopState() {}
	// ステートに入った時のメソッド
	void Start() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override;
private:
	float attack_time = 0.0f;	// 回転攻撃を行う時間
private:
	State::ChangeState change_idle_state;
	State::ChangeState change_dead_state;
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
	std::weak_ptr<CircleCollisionComponent> child_collision_Wprt;
	std::weak_ptr<CharacterComponent> character_Wptr;
	std::weak_ptr<PlayerComponent> player_Wptr;
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
	void Start() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override;
private:
	State::ChangeState change_idle_state;
	State::ChangeState change_dead_state;
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
	std::weak_ptr<CircleCollisionComponent> child_collision_Wprt;
	std::weak_ptr<CharacterComponent> character_Wptr;
};

// 遷移可能な攻撃ステート
class PlayerAttackHoldState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// コンストラクタ
	PlayerAttackHoldState();
	~PlayerAttackHoldState() {}
	// ステートに入った時のメソッド
	void Start() override {};
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	State::ChangeState change_idle_state;
	State::ChangeState change_move_state;
	State::ChangeState change_dead_state;
	State::ChangeState change_spin_attack_state;
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
	std::weak_ptr<CircleCollisionComponent> child_collision_Wprt;
	std::weak_ptr<CharacterComponent> character_Wptr;
	std::weak_ptr<MovementComponent> movement_Wpt;
	std::weak_ptr<PlayerComponent> player_Wptr;
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
	void Start() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override;
private:
	State::ChangeState change_idle_state;
	State::ChangeState change_move_state;
	State::ChangeState change_attack_state;
	State::ChangeState change_spin_attack_state;
	State::ChangeState change_dead_state;
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
	std::weak_ptr<CharacterComponent> character_Wptr;
};

// 死亡ステート
class PlayerDeadState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// コンストラクタ
	PlayerDeadState();
	~PlayerDeadState() {}
	// ステートに入った時のメソッド
	void Start() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	State::ChangeState change_dead_idle_state;
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
};

// 死亡待機ステート
class PlayerDeadIdleState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// コンストラクタ
	PlayerDeadIdleState();
	~PlayerDeadIdleState() {}
	// ステートに入った時のメソッド
	void Start() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override {};
	// ステートから出ていくときのメソッド
	void End() override {};
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
};