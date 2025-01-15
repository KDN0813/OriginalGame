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
	// �R���X�g���N�^
	EnemydleState();
	~EnemydleState() {}
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
	State::ChangeState change_dead_state;
private:
	std::weak_ptr<ModelAnimationControlComponent> animation_Wprt;
	std::weak_ptr<MovementComponent> movement_Wpt;
	std::weak_ptr<CharacterComponent> character_Wptr;
};