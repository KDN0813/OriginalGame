#pragma once
#include "StateMachine\StateBase.h"

class Transform2DComponent;

class LoadingIconAnimationState : public State
{
public:
	// �R���X�g���N�^
	LoadingIconAnimationState();
	~LoadingIconAnimationState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Start() override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override {};

private:
	float timer = 0.0f;
	const float ROTATION_SPEED = 10.0f;

private:
	std::weak_ptr<Transform2DComponent> transform_Wprt;
};