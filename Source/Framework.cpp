#include <memory>
#include <sstream>

#include "Graphics/Graphics.h"
#ifdef _DEBUG
#include "Scene/SceneGame.h"
#include "Scene/SceneTitle.h"
#include "Scene/SceneResult.h"
#else
#include "Scene/SceneTitle.h"
#endif _DEBUG
#include "Scene/SceneManager.h"
#include "Framework.h"

// 垂直同期間隔設定
#ifdef _DEBUG
static const int syncInterval = 0;
#elif  RELEASE_DEBUG
static const int syncInterval = 0;
#else
static const int syncInterval = 1;
#endif // _DEBUG


Framework::Framework(HWND hWnd)
	: hWnd(hWnd)
	, graphics(hWnd)
	, input(hWnd)
#ifdef _DEBUG
	, debug_manager(hWnd, graphics.GetDevice())
#endif // _DEBUG
{
#ifdef _DEBUG
	scene_manager.ChangeScene(new SceneTitle);
	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);  // デフォルト値を再設定
#else
	scene_manager.ChangeScene(new SceneTitle);
#endif // _DEBUG
}

Framework::~Framework()
{
	scene_manager.Clear();
}

void Framework::Update(float elapsed_time)
{
	input.Update();

#ifdef _DEBUG
	if (GetAsyncKeyState('P') & 0x8000)
	{
		this->stop_delta = !this->stop_delta;
	}

	elapsed_time = this->stop_delta ? 0.0f : elapsed_time;
#endif // _DEBUG


	scene_manager.Update(elapsed_time);
}

void Framework::Render(float elapsed_time)
{
	std::lock_guard<std::mutex> lock(graphics.GetMutex());
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();

#ifdef _DEBUG
	debug_manager.GetImGuiRenderer()->NewFrame();
#endif // _DEBUG

	scene_manager.Render();

#ifdef _DEBUG
	DrawDebugGUI();
	debug_manager.GetImGuiRenderer()->Render(dc);
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
	static int frames = 0;
	static float time_tlapsed = 0.0f;

	frames++;

	if ((timer.TimeStamp() - time_tlapsed) >= 1.0f)
	{
		float fps = static_cast<float>(frames); // fps = frameCnt / 1
		float mspf = 1000.0f / fps;
		std::ostringstream outs;
		outs.precision(6);
		outs << "FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
		SetWindowTextA(hWnd, outs.str().c_str());

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
			//if (!IsWindowActive(hWnd)) continue;

			timer.Tick();
#ifdef _DEBUG
			CalculateFrameStats();
#elif RELEASE_DEBUG
			CalculateFrameStats();
#endif // _DEBUG


			float elapsed_time = timer.TimeInterval();
			Update(elapsed_time);
			Render(elapsed_time);
		}
	}
	return static_cast<int>(msg.wParam);
}


LRESULT CALLBACK Framework::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	if (this->debug_manager.GetImGuiRenderer()->HandleMessage(hWnd, msg, wParam, lParam))
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
		timer.Stop();
		break;
	case WM_EXITSIZEMOVE:
		timer.Start();
		break;
	case WM_MOUSEWHEEL:
		Input::Instance()->GetMouse().SetWheel(GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

#ifdef _DEBUG

void Framework::DrawDebugGUI()
{
	// シーン
	this->scene_manager.DrawDebugGUI();

	// カメラ
	CameraManager* camera_manager = CameraManager::Instance();
	if (camera_manager)
	{
		camera_manager->DrawDebugGUI();
	}
}

#endif // DEBUG