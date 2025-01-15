#pragma once
#include "StateMachine\StateBase.h"

class ModelAnimationControlComponent;
class MovementComponent;
class CharacterComponent;

class EnemydleState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// コンストラクタ
	EnemydleState();
	~EnemydleState() {}
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
	State::ChangeState change_dead_state;
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<MovementComponent> movement_Wpt;
	std::weak_ptr<CharacterComponent> character_Wptr;
};