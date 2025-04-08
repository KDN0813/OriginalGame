#pragma once
#include <memory>
#include "StateBase.h"

class ModelAnimationControlComponent;
class MovementComponent;
class PlayerComponent;
class CircleCollisionComponent;
class CharacterComponent;
class Transform3DComponent;

class PlayerIdleState : public State
{
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
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<MovementComponent> movement_Wpt;
	std::weak_ptr<CharacterComponent> character_Wptr;
	std::weak_ptr<PlayerComponent> player_Wptr;
};

class PlayerMoveState : public State
{
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
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<MovementComponent> movement_Wpt;
	std::weak_ptr<CharacterComponent> character_Wptr;
	std::weak_ptr<PlayerComponent> player_Wptr;
};

class PlayerAttackState : public State
{
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
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
	std::weak_ptr<CircleCollisionComponent> child_collision_Wprt;
	std::weak_ptr<CharacterComponent> character_Wptr;
	std::weak_ptr<Transform3DComponent> transform_Wptr;
	std::weak_ptr<Transform3DComponent> attack_object_transform_Wptr;
};

class PlayerAttackComboState : public State
{
public:
	// コンストラクタ
	PlayerAttackComboState();
	~PlayerAttackComboState() {}
	// ステートに入った時のメソッド
	void Start() override;
	// ステートで実行するメソッド
	void Update(float elapsed_time) override;
	// ステートから出ていくときのメソッド
	void End() override;
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
	std::weak_ptr<CircleCollisionComponent> child_collision_Wprt;
	std::weak_ptr<CharacterComponent> character_Wptr;
	std::weak_ptr<Transform3DComponent> transform_Wptr;
	std::weak_ptr<Transform3DComponent> attack_object_transform_Wptr;
};

// 回転攻撃開始ステート
class PlayerSpinAttackStartState : public State
{
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
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
	std::weak_ptr<CircleCollisionComponent> child_collision_Wprt;
	std::weak_ptr<CharacterComponent> character_Wptr;
};

// 回転攻撃(ループ)
class PlayerSpinAttackSpinLoopState : public State
{
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
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
	std::weak_ptr<CircleCollisionComponent> child_collision_Wprt;
	std::weak_ptr<CharacterComponent> character_Wptr;
	std::weak_ptr<PlayerComponent> player_Wptr;
	std::weak_ptr<Transform3DComponent> transform_Wptr;
	std::weak_ptr<Transform3DComponent> attack_object_transform_Wptr;
};

// 回転攻撃
class PlayerSpinAttackState : public State
{
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
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
	std::weak_ptr<CircleCollisionComponent> child_collision_Wprt;
	std::weak_ptr<CharacterComponent> character_Wptr;
};

// 遷移可能な攻撃ステート
class PlayerAttackHoldState : public State
{
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
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
	std::weak_ptr<CharacterComponent> character_Wptr;
};

// 死亡ステート
class PlayerDeadState : public State
{
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
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
};

// 死亡待機ステート
class PlayerDeadIdleState : public State
{
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