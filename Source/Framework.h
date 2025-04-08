#pragma once

#include <windows.h>
#include "System/GameTimer.h"
#include "Graphics/Graphics.h"
#include "Scene/SceneManager.h"
#include "Input/Input.h"
#include "Model/ModelResourceManager.h"
#include "Model/InstancingModelResourceManager.h"
#include "Debug/DebugManager.h"
#include "Camera/CameraManager.h"
#include "Camera/CameraManager.h"
#include "System/GameData.h"
#include "Audio/Audio.h"
#include "Audio/AudioResourceManager.h"
#include "Shader\SpriteShader.h"
#include "Effekseer\EffekseerSystem.h"

class Framework
{
public:
	Framework(HWND hWnd);
	~Framework();

public:
	int Run();
	LRESULT CALLBACK HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	void Update(float elapsed_time);
	void Render(float elapsed_time);

	// ウィンドウ終了指示がきているか
	bool IsWindowClose();
	// ポーズキーが押されているか
	bool IsPressedPauseKey();
private:
	const HWND						hWnd;
	HDC								hDC;

	Graphics						graphics;
	SceneManager					scene_manager;
	Input							input;
	ModelResourceManager			resource_manager;
	InstancingModelResourceManager	instancin_resource_manager;
	AudioResourceManager			audio_resource_manager;
	CameraManager					camera_manager;
	GameData						game_data;
	Audio							audio;
	SpriteShader					sprite_shader;
	EffekseerSystem				effekseer_manager;
	BOOL fullscreen_mode{ FALSE };
	GameTimer				game_timer;
#ifdef _DEBUG
public:
	void DrawDebugGUI();

private:
	DebugManager					debug_manager;
#endif // _DEBUG
};