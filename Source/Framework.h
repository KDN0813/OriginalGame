#pragma once

#include <windows.h>
#include "System/HighResolutionTimer.h"
#include "Graphics/Graphics.h"
#include "Scene/SceneManager.h"
#include "Input/Input.h"
#include "Model/ModelResourceManager.h"
#include "Debug/DebugManager.h"

class Framework
{
public:
	Framework(HWND hWnd);
	~Framework();

private:
	void Update(float elapsed_time);
	void Render(float elapsed_time);

	void CalculateFrameStats();
public:
	int Run();
	LRESULT CALLBACK HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	const HWND				hWnd;
	HighResolutionTimer		timer;
	Graphics				graphics;
	SceneManager			sceneManager;
	Input					input;
	ModelResourceManager	resourceManager;
#ifdef _DEBUG
	DebugManager			debugManager;
#endif // _DEBUG
};