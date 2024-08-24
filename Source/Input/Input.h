#pragma once

#include "Input/GamePad.h"
#include "Input/Mouse.h"
#include "System/ClassBase/Singleton.h"

// インプット
class Input : public Singleton<Input>
{
public:
	Input(HWND hWnd);
	~Input() override{}

public:
	// 更新処理
	void Update();

	// ゲームパッド取得
	GamePad& GetGamePad() { return game_pad; }

	// マウス取得
	Mouse& GetMouse() { return mouse; }

private:
	GamePad				game_pad;
	Mouse				mouse;
};
