#pragma once
#include <memory>
#include "System/MyHash.h"

class Object;

// �X�e�[�g�}�V���Ŏg�p����X�e�[�g�̊��N���X
class State
{
public:
	using StateIndex = size_t;
	static const StateIndex INVALID_STATE_INDEX = SIZE_MAX;	// �����ȃX�e�[�g�̃C���f�b�N�X(�X�e�[�g�̃C���f�b�N�X�̏����l�E�G���[�R�[�h�Ɏg�p����)

	// �X�e�[�g�}�V���ŃX�e�[�g��؂�ւ���ۂɎg�p�������ێ�����\���́B
	// �X�e�[�g���̃n�b�V���l�i���O�ɂ�錟���p�j�ƁA�X�e�[�g�̃C���f�b�N�X�i����̌�����Ɏg�p�j��ێ�����B
	struct ChangeState
	{
		MyHash change_state_name;								// �X�e�[�g���̃n�b�V���l�B�ŏ��̃X�e�[�g�������Ɏg�p�����B
		StateIndex change_state_index = INVALID_STATE_INDEX;	// �X�e�[�g�̃C���f�b�N�X�B�ŏ��̌�����͂��̃C���f�b�N�X�ŃX�e�[�g���Q�Ƃ���B(�����l�͍ő�l)
	};
public:
	// �R���X�g���N�^
	State() = delete;
	State(std::string name) : state_name(name) {}
	virtual ~State() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	virtual void Staet() = 0;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	virtual void Update(float elapsed_time) = 0;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	virtual void End() = 0;

	// �n�b�V���̎擾
	MyHash GetHash() const { return this->state_name; }
	// ���������L���Ă���X�e�[�g�̔z��̎��g�̃C���f�b�N�X�̎擾
	StateIndex GetStateIndex() { return this->state_index; }
	// ���������L���Ă���X�e�[�g�̔z��̎��g�̃C���f�b�N�X�̐ݒ�
	void SetStateIndex(StateIndex index) { this->state_index = index; }

	// ���L�҂̐ݒ�
	void SetOwner(const std::shared_ptr<Object>& owner) { this->owner = owner; }
protected:
	std::weak_ptr<Object> owner;	// ���L�҂̃|�C���^
	MyHash state_name;				// �n�b�V��()
	StateIndex state_index = INVALID_STATE_INDEX;	// �������������Ă���X�e�[�g�z��̃C���f�b�N�X
};

class DefaultState : public State
{
public:
	// �R���X�g���N�^
	DefaultState() :State("DefaultState") {};
	~DefaultState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Staet() override{};
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Update(float elapsed_time) override {};
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void End() override {};
};
