#include <memory>
#include <sstream>

#include "Graphics/Graphics.h"
#include "Scene/SceneTest.h"
#include "Scene/SceneTitle.h"
#include "Scene/SceneManager.h"
#include "Framework.h"

// ���������Ԋu�ݒ�
static const int syncInterval = 1;

Framework::Framework(HWND hWnd)
	: hWnd(hWnd)
	, graphics(hWnd)
#ifdef _DEBUG
	, debugManager(hWnd, graphics.GetDevice())
#endif // _DEBUG
{
#ifdef _DEBUG
	sceneManager.ChangeScene(new SceneTest);
#else
	SceneManager::Instance().ChangeScene(new SceneTitle);
#endif // _DEBUG
}

Framework::~Framework()
{
	sceneManager.Clear();
}

void Framework::Update(float elapsed_time)
{
	sceneManager.Update(elapsed_time);
}

void Framework::Render(float elapsed_time)
{
	std::lock_guard<std::mutex> lock(graphics.GetMutex());
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();

#ifdef _DEBUG
	debugManager.GetImGuiRenderer()->NewFrame();
#endif // _DEBUG

	sceneManager.Render();

#ifdef _DEBUG
	debugManager.GetImGuiRenderer()->Render(dc);
#endif // _DEBUG

	graphics.GetSwapChain()->Present(syncInterval, 0);
}

bool IsWindowActive(HWND hwnd) 
{
	return (GetForegroundWindow() == hwnd);
}

int Framework::Run()
{
	MSG msg = {};

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (!IsWindowActive(hWnd)) continue;

			timer.Tick();

			float elapsed_time = syncInterval == 0
				? timer.TimeInterval()
				: syncInterval / 60.0f
				;
			Update(elapsed_time);
			Render(elapsed_time);
		}
	}
	return static_cast<int>(msg.wParam);
}


LRESULT CALLBACK Framework::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	if (this->debugManager.GetImGuiRenderer()->HandleMessage(hWnd, msg, wParam, lParam))
		return true;
#endif _DEBUG

	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostMessage(hWnd, WM_CLOSE, 0, 0);
		break;
	case WM_ENTERSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
		timer.Stop();
		break;
	case WM_EXITSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
		timer.Start();
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}