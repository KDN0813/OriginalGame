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
	void Staet() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
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
	// �R���X�g���N�^
	PlayerMoveState();
	~PlayerMoveState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Staet() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
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
	// �R���X�g���N�^
	PlayerAttackState();
	~PlayerAttackState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Staet() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override;
private:
	State::ChangeState change_idle_state;
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
	void Staet() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override;
private:
	State::ChangeState change_idle_state;
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
	std::weak_ptr<CircleCollisionComponent> child_collision_Wprt;
	std::weak_ptr<CharacterComponent> character_Wptr;
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
	void Staet() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
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