#pragma once
#include <memory>
#include "System/MyHash.h"

class Object;

// �X�e�[�g�}�V���Ŏg�p����X�e�[�g�̊��N���X
class State
{
public:
	// �R���X�g���N�^
	State() = delete;
	State(std::string name) : state_name(name) {}
	virtual ~State() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	virtual void Enter() = 0;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	virtual void Execute(float elapsed_time) = 0;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	virtual void Exit() = 0;

	// ���L�҂̐ݒ�
	void SetOwner(const std::shared_ptr<Object>& owner) { this->owner = owner; }
protected:
	std::weak_ptr<Object> owner;	// ���L�҂̃|�C���^
	MyHash state_name;
};

class DefaultState : public State
{
public:
	// �R���X�g���N�^
	DefaultState() :State("DefaultState") {};
	~DefaultState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter() override{};
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsed_time) override {};
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit() override {};
};
