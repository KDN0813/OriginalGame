#pragma once
#include "StateMachine\StateBase.h"

class SpriteComponent;

class LoadingIconAnimationState : public State
{
public:
	// �R���X�g���N�^
	LoadingIconAnimationState();
	~LoadingIconAnimationState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Staet() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override {};

private:
	float timer = 0.0f;

private:
	std::weak_ptr<SpriteComponent> sprite_Wprt;
};