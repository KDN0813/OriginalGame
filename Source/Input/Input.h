#pragma once

#include "Input/GamePad.h"
#include "Input/Mouse.h"
#include "System/ClassBase/Singleton.h"

// �C���v�b�g
class Input : public Singleton<Input>
{
public:
	Input(HWND hWnd);
	~Input() override{}

public:
	// �X�V����
	void Update();

	// �Q�[���p�b�h�擾
	GamePad& GetGamePad() { return game_pad; }

	// �}�E�X�擾
	Mouse& GetMouse() { return mouse; }

private:
	GamePad				game_pad;
	Mouse				mouse;
};
