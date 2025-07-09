#pragma once

using GamePadButton = unsigned int;

// �Q�[���p�b�h
class GamePad
{
public:
	static const GamePadButton BTN_UP				= (1 <<  0);
	static const GamePadButton BTN_RIGHT			= (1 <<  1);
	static const GamePadButton BTN_DOWN				= (1 <<  2);
	static const GamePadButton BTN_LEFT				= (1 <<  3);
	static const GamePadButton BTN_A				= (1 <<  4);
	static const GamePadButton BTN_B				= (1 <<  5);
	static const GamePadButton BTN_X				= (1 <<  6);
	static const GamePadButton BTN_Y				= (1 <<  7);
	static const GamePadButton BTN_START			= (1 <<  8);
	static const GamePadButton BTN_BACK				= (1 <<  9);
	static const GamePadButton BTN_LEFT_THUMB		= (1 << 10);
	static const GamePadButton BTN_RIGHT_THUMB		= (1 << 11);
	static const GamePadButton BTN_LEFT_SHOULDER	= (1 << 12);
	static const GamePadButton BTN_RIGHT_SHOULDER	= (1 << 13);
	static const GamePadButton BTN_LEFT_TRIGGER		= (1 << 14);
	static const GamePadButton BTN_RIGHT_TRIGGER	= (1 << 15);
	static const GamePadButton BTN_EXIT				= (1 << 16);			// ESC�L�[��p
#ifdef _DEBUG
	static const GamePadButton BTN_DEBUG_IMGUI				= (1 << 17);	// IMGUI�̕\���E��\���Ɏg���{�^��
	static const GamePadButton BTN_DEBUG_PRIMITIVE			= (1 << 18);	// �f�o�b�O�v���~�e�B�u�̕\���E��\���Ɏg���{�^��
	static const GamePadButton BTN_DEBUG_RETURN_TO_TITLE	= (1 << 19);	// �f�o�b�O�p�̃^�C�g���ɖ߂�{�^��
	static const GamePadButton BTN_DEBUG_GAME_RESET			= (1 << 20);	// �f�o�b�O�p�̃Q�[�����Z�b�g�{�^��
	static const GamePadButton BTN_DEBUG_DEMO				= (1 << 21);	// �f�o�b�O�p�̃f����Ԃ�ύX����t���O
	static const GamePadButton BTN_DEBUG_FORCE_EXIT			= (1 << 22);	// �f�o�b�O�p�̋����I���{�^��
	static const GamePadButton BTN_DEBUG_PLAYER_INVINCIBLE	= (1 << 24);	// �f�o�b�O�p�̃v���C���[���G����
	static const GamePadButton BTN_DEBUG_STOP_TIMER			= (1 << 25);	// �f�o�b�O�p�̃^�C�}�[�̒�~
	static const GamePadButton BTN_DEBUG_GAME_SPEED_TOGGLE	= (1 << 26);	// �Q�[���X�s�[�h��0�i��~�j��1�i�ʏ�j�Ő؂�ւ���
#endif // _DEBUG

public:
	GamePad() {}
	~GamePad() {}

	// �X�V
	void Update();

	// �X���b�g�ݒ�
	void SetSlot(int slot) { this->slot = slot; }

	// �{�^�����͏�Ԃ̎擾
	GamePadButton GetButton() const { return button_state[0]; }

	// �{�^��������Ԃ̎擾
	GamePadButton GetButtonDown() const { return button_down; }

	// �{�^�������Ԃ̎擾
	GamePadButton GetButtonUp() const { return button_up; }

	// ���X�e�B�b�NX�����͏�Ԃ̎擾
	float GetAxisLX() const { return axisLx; }

	// ���X�e�B�b�NY�����͏�Ԃ̎擾
	float GetAxisLY() const { return axisLy; }

	// �E�X�e�B�b�NX�����͏�Ԃ̎擾
	float GetAxisRX() const { return axisRx; }

	// �E�X�e�B�b�NY�����͏�Ԃ̎擾
	float GetAxisRY() const { return axisRy; }

	// ���g���K�[���͏�Ԃ̎擾
	float GetTriggerL() const { return triggerL; }

	// �E�g���K�[���͏�Ԃ̎擾
	float GetTriggerR() const { return triggerR; }

private:
	GamePadButton		button_state[2] = { 0 };
	GamePadButton		button_down = 0;
	GamePadButton		button_up = 0;
	float				axisLx = 0.0f;
	float				axisLy = 0.0f;
	float				axisRx = 0.0f;
	float				axisRy = 0.0f;
	float				triggerL = 0.0f;
	float				triggerR = 0.0f;
	int					slot = 0;
};
