#pragma once
#include "StateMachine\StateBase.h"

class TextNumberComponent;

// �X�R�AUI�̒ʏ�X�e�[�g
class ScoreUIDefaultState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// �R���X�g���N�^
	ScoreUIDefaultState();
	~ScoreUIDefaultState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Staet() override {};
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override {};
private:
	std::weak_ptr<TextNumberComponent> text_number_Wptr;
};

// �I���^�C�}�[�̒ʏ�X�e�[�g
class EndTimerUIDefaultState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// �R���X�g���N�^
	EndTimerUIDefaultState();
	~EndTimerUIDefaultState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Staet() override {};
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override {};
private:
	std::weak_ptr<TextNumberComponent> text_number_Wptr;
};
