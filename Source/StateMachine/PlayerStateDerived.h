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
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<MovementComponent> movement_Wpt;
	std::weak_ptr<CharacterComponent> character_Wptr;
	std::weak_ptr<PlayerComponent> player_Wptr;
};

class PlayerMoveState : public State
{
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
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<MovementComponent> movement_Wpt;
	std::weak_ptr<CharacterComponent> character_Wptr;
	std::weak_ptr<PlayerComponent> player_Wptr;
};

class PlayerAttackState : public State
{
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
	// �R���X�g���N�^
	PlayerAttackComboState();
	~PlayerAttackComboState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Start() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override;
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
	std::weak_ptr<CircleCollisionComponent> child_collision_Wprt;
	std::weak_ptr<CharacterComponent> character_Wptr;
	std::weak_ptr<Transform3DComponent> transform_Wptr;
	std::weak_ptr<Transform3DComponent> attack_object_transform_Wptr;
};

// ��]�U���J�n�X�e�[�g
class PlayerSpinAttackStartState : public State
{
public:
	// �R���X�g���N�^
	PlayerSpinAttackStartState();
	~PlayerSpinAttackStartState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Start() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override {};
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
	std::weak_ptr<CircleCollisionComponent> child_collision_Wprt;
	std::weak_ptr<CharacterComponent> character_Wptr;
};

// ��]�U��(���[�v)
class PlayerSpinAttackSpinLoopState : public State
{
public:
	// �R���X�g���N�^
	PlayerSpinAttackSpinLoopState();
	~PlayerSpinAttackSpinLoopState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Start() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
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

// ��]�U��
class PlayerSpinAttackState : public State
{
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
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
	std::weak_ptr<CircleCollisionComponent> child_collision_Wprt;
	std::weak_ptr<CharacterComponent> character_Wptr;
};

// �J�ډ\�ȍU���X�e�[�g
class PlayerAttackHoldState : public State
{
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
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
	std::weak_ptr<CircleCollisionComponent> child_collision_Wprt;
	std::weak_ptr<CharacterComponent> character_Wptr;
	std::weak_ptr<MovementComponent> movement_Wpt;
	std::weak_ptr<PlayerComponent> player_Wptr;
};

// �_���[�W�X�e�[�g
class PlayerDamageState : public State
{
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
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
	std::weak_ptr<CharacterComponent> character_Wptr;
};

// ���S�X�e�[�g
class PlayerDeadState : public State
{
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
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<PlayerComponent> player_Wprt;
};

// ���S�ҋ@�X�e�[�g
class PlayerDeadIdleState : public State
{
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