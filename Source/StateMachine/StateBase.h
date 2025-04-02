#pragma once
#include <memory>

class Object;
class StateMachineComponent;

// �X�e�[�g�}�V���Ŏg�p����X�e�[�g�̊��N���X
class State
{
public:
	// �R���X�g���N�^
	State() = default;
	virtual ~State() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	virtual void Start() = 0;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	virtual void Update(float elapsed_time) = 0;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	virtual void End() = 0;

	// ���L�҂̐ݒ�
	void SetOwner(const std::shared_ptr<Object>& owner) { this->owner = owner; }
	// ���L�҂̎擾
	std::shared_ptr<Object> GetOwner() const { return this->owner.lock(); }
protected:
	std::weak_ptr<Object> owner;								// ���L�҂̃|�C���^
	std::weak_ptr<StateMachineComponent> state_machine_Wptr;	// �������������Ă���X�e�[�g�}�V���̃|�C���^
};

class DefaultState : public State
{
public:
	// �R���X�g���N�^
	DefaultState() {};
	~DefaultState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Start() override{};
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override {};
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override {};
};
