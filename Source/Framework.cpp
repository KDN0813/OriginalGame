#include <memory>
#include <sstream>

#include "Graphics/Graphics.h"

#ifdef _DEBUG
#include "Scene/SceneTitle.h"
#include "Scene/SceneResult.h"
#include <imgui.h>
#endif

#if defined(_DEBUG) || defined(RELEASE_DEBUG)
#include "Scene/SceneGame.h"
#endif

#include "Scene/SceneTitle.h"
#include "Scene/SceneManager.h"
#include "Framework.h"

Framework::Framework(HWND hWnd)
	: hWnd(hWnd)
	, hDC(GetDC(hWnd))
	, graphics(hWnd)
	, input(hWnd)
#ifdef _DEBUG
	, debug_manager(hWnd)
#endif // _DEBUG
{
	scene_manager.Initialize();
	effekseer_manager.Initialize();
#if defined(_DEBUG)
	scene_manager.ChangeScene(new SceneGame);
	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.4f, 0.4f, 0.4f, 0.85f);  // デフォルト値を再設定
#elif defined(RELEASE_DEBUG)
	scene_manager.ChangeScene(new SceneGame);
#else
	scene_manager.ChangeScene(new SceneTitle);
#endif // _DEBUG
}

Framework::~Framework()
{
	scene_manager.Clear();
	effekseer_manager.Finalize();
	ReleaseDC(hWnd, hDC);
}

void Framework::Update(float elapsed_time)
{
	input.Update();

#ifdef _DEBUG
	// キー入力によるデバッグ用機能
	DebugManager::GetInstance()->HandleFunctionKeyDebug();

	// フレームレート計算
	game_timer.CalculateFrameStats();
#endif // _DEBUG

	scene_manager.Update(elapsed_time);

	// ウィンドウの終了判定
	if (IsWindowClose())
	{
		PostMessage(hWnd, WM_CLOSE, 0, 0);
	}
}

void Framework::Render(float elapsed_time)
{
#ifdef _DEBUG
	if (GameData::Instance game_data = GameData::GetInstance(); (game_data->GetDrawImguiFlag()))
	{
		debug_manager.GetImGuiRenderer()->NewFrame();
	}
#endif // _DEBUG

	scene_manager.Render();

#ifdef _DEBUG
	if (GameData::Instance game_data = GameData::GetInstance(); (game_data->GetDrawImguiFlag()))
	{
		DrawDebugGUI();
		debug_manager.GetImGuiRenderer()->Render();
	}
#endif // _DEBUG

	const UINT sync_interval = static_cast<UINT>(game_timer.GetSyncInterval());
	UINT flags = (graphics.GetTearingSupported() && !this->fullscreen_mode && !sync_interval) ? DXGI_PRESENT_ALLOW_TEARING : 0;
	graphics.GetSwapChain()->Present(sync_interval, flags);
}

bool IsWindowActive(HWND hwnd)
{
	return (GetForegroundWindow() == hwnd);
}

bool Framework::IsWindowClose()
{
	GameData::Instance game_data = GameData::GetInstance();
	return (game_data->GetIsCloseWindow());
}

bool Framework::IsPressedPauseKey()
{
	GamePad& game_pad = input.GetGamePad();
	return (GamePad::BTN_START & game_pad.GetButtonDown());
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
			game_timer.Tick();
			if (!IsWindowActive(hWnd)) continue;

			if (IsPressedPauseKey())
			{
				GameData::Instance game_data = GameData::GetInstance();
				if (game_data->GetGameStatus() == GameData::GameStatus::GAME)
				{
					game_data->SetIsPause(!game_data->GetIsPause());
					this->scene_manager.GetPauseObject()->SetIsActive(game_data->GetIsPause());	// pause用オブジェクトの(非)アクティブ化
				}
			}

			float elapsed_time = game_timer.GetDeltaTime();
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
	case WM_CLOSE:
	{
		GameData::Instance game_data = GameData::GetInstance();
		// ロード中ならウィンドウを閉じない
		if (game_data->GetIsLoading()) break;
	}
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		break;
	case WM_KEYDOWN:
		break;
	case WM_ENTERSIZEMOVE:
		game_timer.TimerStop();
		break;
	case WM_EXITSIZEMOVE:
		game_timer.TimerStart();
		break;
	case WM_MOUSEWHEEL:
	{
		Input::Instance input = Input::GetInstance();
		input->GetMouse().SetWheel(GET_WHEEL_DELTA_WPARAM(wParam));
	}
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
	{
		CameraManager::Instance camera_manager = CameraManager::GetInstance();
		camera_manager->DrawDebugGUI();
	}

	// オーディオ
	{
		Audio::Instance audio = Audio::GetInstance();
		audio->DebugDrawGUI();
	}

	// ゲームデータ
	{
		GameData::Instance game_data = GameData::GetInstance();
		game_data->DebugDrawGUI();
	}

	// FPS
	{
		game_timer.DebugDrawGUI();
	}
}

#endif // DEBUG