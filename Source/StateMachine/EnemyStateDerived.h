#pragma once
#include "StateMachine\StateBase.h"

class ModelAnimationControlComponent;
class MovementComponent;
class CharacterComponent;

// �ҋ@�X�e�[�g
class EnemyIdleState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// �R���X�g���N�^
	EnemyIdleState();
	~EnemyIdleState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Start() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
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

// �p�j�X�e�[�g
class EnemyWanderingState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// �R���X�g���N�^
	EnemyWanderingState();
	~EnemyWanderingState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Start() override {};
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override {};
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override {};
private:
	State::ChangeState change_idle_state;
	State::ChangeState change_chase_state;
	State::ChangeState change_attack_state;
	State::ChangeState change_damage_state;
	State::ChangeState change_deth_state;
};

// �ڋ߃X�e�[�g
class EnemyChaseState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// �R���X�g���N�^
	EnemyChaseState();
	~EnemyChaseState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Start() override {};
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override {};
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override {};
private:
	State::ChangeState change_idle_state;
	State::ChangeState change_chase_state;
	State::ChangeState change_attack_state;
};

// �U���X�e�[�g
class EnemyAttackState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// �R���X�g���N�^
	EnemyAttackState();
	~EnemyAttackState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Start() override {};
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override {};
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override {};
private:
	State::ChangeState change_idle_state;
};

// �_���[�W�X�e�[�g
class EnemyDamageState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// �R���X�g���N�^
	EnemyDamageState();
	~EnemyDamageState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Start() override {};
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override {};
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override {};
private:
	State::ChangeState change_idle_state;
};

// ���S�X�e�[�g
class EnemyDethState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// �R���X�g���N�^
	EnemyDethState();
	~EnemyDethState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Start() override {};
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override {};
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override {};
private:
	State::ChangeState change_deth_idle_state;
};

// ���S�ҋ@�X�e�[�g
class EnemyDethIdleState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// �R���X�g���N�^
	EnemyDethIdleState();
	~EnemyDethIdleState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Start() override {};
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override {};
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override {};
private:
};