#include "Input/Input.h"

// �R���X�g���N�^
Input::Input(HWND hWnd)
	:Singleton(this)
	, mouse(hWnd)
{
}

// �X�V����
void Input::Update()
{
	game_pad.Update();
	mouse.Update();
}
