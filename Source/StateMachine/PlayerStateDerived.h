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
	// �R���X�g���N�^
	PlayerIdleState();
	~PlayerIdleState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Start() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
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
};

class PlayerMoveState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// �R���X�g���N�^
	PlayerMoveState();
	~PlayerMoveState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Start() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
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
};

class PlayerAttackState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// �R���X�g���N�^
	PlayerAttackState();
	~PlayerAttackState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Start() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
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
	// �R���X�g���N�^
	PlayerAttackLCombo2State();
	~PlayerAttackLCombo2State() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Start() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
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

// ��]�U��
class PlayerSpinAttackState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// �R���X�g���N�^
	PlayerSpinAttackState();
	~PlayerSpinAttackState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Start() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
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

// �J�ډ\�ȍU���X�e�[�g
class PlayerAttackHoldState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// �R���X�g���N�^
	PlayerAttackHoldState();
	~PlayerAttackHoldState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Start() override {};
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
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
};

// �_���[�W�X�e�[�g
class PlayerDamageState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// �R���X�g���N�^
	PlayerDamageState();
	~PlayerDamageState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Start() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
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

// ���S�X�e�[�g
class PlayerDeadState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// �R���X�g���N�^
	PlayerDeadState();
	~PlayerDeadState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Start() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override {};
private:
	State::ChangeState change_dead_idle_state;
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
};

// ���S�ҋ@�X�e�[�g
class PlayerDeadIdleState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// �R���X�g���N�^
	PlayerDeadIdleState();
	~PlayerDeadIdleState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Start() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override {};
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override {};
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
};