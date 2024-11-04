#pragma once
#include <memory>
#include "StateBase.h"

class ModelAnimationControlComponent;
class MovementComponent;

class PlayerIdleState : public State
{
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
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<MovementComponent> movement_Wpt;
};

class PlayerMoveState : public State
{
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
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<MovementComponent> movement_Wpt;
};