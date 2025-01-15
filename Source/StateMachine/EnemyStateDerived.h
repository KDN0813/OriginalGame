#pragma once
#include <DirectXMath.h>
#include <memory>
#include "StateMachine\StateBase.h"

class InstancedModelWithAnimationComponent;
class CharacterComponent;
class EnemyComponent;
class Transform3DComponent;
class CircleCollisionComponent;

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
	// �ҋ@���Ԃ�ݒ肷��
	void SetRandomIdleTime();
private:
	float idle_timer = 0.0f;	// �ҋ@����
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
	void Start() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
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
	void Start() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override;
private:
	State::ChangeState change_idle_state;
	State::ChangeState change_chase_state;
	State::ChangeState change_attack_state;
private:
	std::weak_ptr<InstancedModelWithAnimationComponent> animation_Wprt;
	std::weak_ptr<EnemyComponent> enemy_Wptr;
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
	void Start() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override;
private:
	State::ChangeState change_idle_state;
	State::ChangeState change_chase_state;
private:
	std::weak_ptr<InstancedModelWithAnimationComponent> animation_Wprt;
	std::weak_ptr<CircleCollisionComponent> child_collision_Wprt;
	std::weak_ptr<EnemyComponent> enemy_Wptr;
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
	void Start() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override {};
private:
	State::ChangeState change_idle_state;
private:
	std::weak_ptr<InstancedModelWithAnimationComponent> animation_Wprt;
};

// ���S�X�e�[�g
class EnemyDeadState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// �R���X�g���N�^
	EnemyDeadState();
	~EnemyDeadState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Start() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override {};
private:
	State::ChangeState change_deth_idle_state;
private:
	std::weak_ptr<InstancedModelWithAnimationComponent> animation_Wprt;
};

// ���S�ҋ@�X�e�[�g
class EnemyDeadIdleState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// �R���X�g���N�^
	EnemyDeadIdleState();
	~EnemyDeadIdleState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Start() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override {};
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override {};
private:
	std::weak_ptr<InstancedModelWithAnimationComponent> animation_Wprt;
};