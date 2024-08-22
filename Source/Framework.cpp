#include <memory>
#include <sstream>

#include "Graphics/Graphics.h"
#include "Scene/SceneDebug.h"
#include "Scene/SceneTitle.h"
#include "Scene/SceneManager.h"
#include "Framework.h"

// 垂直同期間隔設定
static const int syncInterval = 0;

Framework::Framework(HWND hWnd)
	: hWnd(hWnd)
	, graphics(hWnd)
	, input(hWnd)
#ifdef _DEBUG
	, debugManager(hWnd, graphics.GetDevice())
#endif // _DEBUG
{
#ifdef _DEBUG
	scene_manager.ChangeScene(new SceneDebug);
#else
	scene_manager.ChangeScene(new SceneDebug);
#endif // _DEBUG
}

Framework::~Framework()
{
	scene_manager.Clear();
}

void Framework::Update(float elapsed_time)
{
	input.Update();

	scene_manager.Update(elapsed_time);
}

void Framework::Render(float elapsed_time)
{
	std::lock_guard<std::mutex> lock(graphics.GetMutex());
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();

#ifdef _DEBUG
	debugManager.GetImGuiRenderer()->NewFrame();
#endif // _DEBUG

	scene_manager.Render();

#ifdef _DEBUG
	debugManager.GetImGuiRenderer()->Render(dc);
#endif // _DEBUG

	graphics.GetSwapChain()->Present(syncInterval, 0);
}

bool IsWindowActive(HWND hwnd) 
{
	return (GetForegroundWindow() == hwnd);
}

// フレームレート計算
void Framework::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.
	static int frames = 0;
	static float time_tlapsed = 0.0f;

	frames++;

	// Compute averages over one second period.
	if ((timer.TimeStamp() - time_tlapsed) >= 1.0f)
	{
		float fps = static_cast<float>(frames); // fps = frameCnt / 1
		float mspf = 1000.0f / fps;
		std::ostringstream outs;
		outs.precision(6);
		outs << "FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
		SetWindowTextA(hWnd, outs.str().c_str());

		// Reset for next average.
		frames = 0;
		time_tlapsed += 1.0f;
	}
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
#ifdef _DEBUG
			CalculateFrameStats();
#endif // _DEBUG


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
	case WM_MOUSEWHEEL:
		Input::Instance().GetMouse().SetWheel(GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
