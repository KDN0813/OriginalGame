#pragma once
#include "StateMachine\StateBase.h"

class TextNumberComponent;
class SpriteComponent;
class CharacterComponent;

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
	void Start() override {};
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
	void Start() override {};
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override {};
private:
	std::weak_ptr<TextNumberComponent> text_number_Wptr;
};

// �I���^�C�}�[�̒ʏ�X�e�[�g
class PlayerHPBarUIState : public State
{
public:
	static const MyHash STATE_NAME;
public:
	// �R���X�g���N�^
	PlayerHPBarUIState();
	~PlayerHPBarUIState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Start() override {};
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override {};
private:
	// �̗̓o�[�̕����v�Z����
	float CalculateHealthBarWidth();
private:
	std::weak_ptr<SpriteComponent> sprite_Wptr;
	std::weak_ptr<CharacterComponent> player_health_Wptr;
};
