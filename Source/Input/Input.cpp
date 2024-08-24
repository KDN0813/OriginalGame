#include "Input/Input.h"

// コンストラクタ
Input::Input(HWND hWnd)
	:Singleton(this)
	, mouse(hWnd)
{
}

// 更新処理
void Input::Update()
{
	game_pad.Update();
	mouse.Update();
}
