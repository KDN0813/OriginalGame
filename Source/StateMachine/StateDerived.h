#pragma once
#include "StateBase.h"

class IdleState : public State
{
public:
	// �R���X�g���N�^
	IdleState() :State("IdleState") {};
	~IdleState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Staet() override {};
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override {};
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override {};
};
