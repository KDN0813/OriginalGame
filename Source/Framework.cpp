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
	, graphics(hWnd)
	, input(hWnd)
#ifdef _DEBUG
	, debug_manager(hWnd)
#endif // _DEBUG
{
	hDC = GetDC(hWnd);
#if defined(_DEBUG)
	scene_manager.ChangeScene(new SceneGame);
	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);  // デフォルト値を再設定
#elif defined(RELEASE_DEBUG)
	scene_manager.ChangeScene(new SceneGame);
#else
	scene_manager.ChangeScene(new SceneTitle);
#endif // _DEBUG
}

Framework::~Framework()
{
	scene_manager.Clear();
	ReleaseDC(hWnd, hDC);
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


#if defined(_DEBUG) || defined(RELEASE_DEBUG)
	// フレームレート計算
	CalculateFrameStats();
#endif

	scene_manager.Update(elapsed_time);
}

void Framework::Render(float elapsed_time)
{
#ifdef _DEBUG
	debug_manager.GetImGuiRenderer()->NewFrame();
#endif // _DEBUG

	scene_manager.Render();

#ifdef _DEBUG
	DrawDebugGUI();
	debug_manager.GetImGuiRenderer()->Render();
#endif // _DEBUG

	graphics.GetSwapChain()->Present(this->sync_interval, 0);
}

bool IsWindowActive(HWND hwnd)
{
	return (GetForegroundWindow() == hwnd);
}

// フレームレート計算
void Framework::CalculateFrameStats()
{
	frames++;

	if ((timer.TimeStamp() - time_tlapsed) >= 1.0f)
	{
		fps = static_cast<float>(frames); // fps = frameCnt / 1
		mspf = 1000.0f / fps;
		frames = 0;
		time_tlapsed += 1.0f;
		std::string str;
		str = "Fps : ";
		str += std::to_string(fps);
		str += "\n";
		OutputDebugStringA(str.c_str());
	}
}

bool Framework::IsPressedWindowCloseKey()
{
	return (GetAsyncKeyState(VK_ESCAPE) & 0x8000);
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
			if (!IsWindowActive(hWnd)) continue;

			timer.Tick();

			// ポーズキーが押されたらポーズ設定(解除)する
			// TODO ポーズ画面が完成したらコメントアウトを外す
			//if (IsPressedPauseKey())
			//{
			//	if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
			//	{
			//		game_data->SetIsPause(!game_data->GetIsPause());
			//	}
			//}

			float elapsed_time = (this->sync_interval == 0)
				? timer.TimeInterval()
				: this->sync_interval / static_cast<float>(GetDeviceCaps(hDC, VREFRESH));
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
		if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
		{
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
		// 終了キーが押されていればウィンドウを閉じる
		if (IsPressedWindowCloseKey()) PostMessage(hWnd, WM_CLOSE, 0, 0);
		break;
	case WM_ENTERSIZEMOVE:
		timer.Stop();
		break;
	case WM_EXITSIZEMOVE:
		timer.Start();
		break;
	case WM_MOUSEWHEEL:
		if (Input::Instance input = Input::GetInstance(); input.Get())
		{
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
	if (CameraManager::Instance camera_manager = CameraManager::GetInstance(); camera_manager.Get())
	{
		camera_manager->DrawDebugGUI();
	}

	// オーディオ
	if (Audio::Instance audio = Audio::GetInstance(); audio.Get())
	{
		audio->DebugDrawGUI();
	}

	// ゲームデータ
	if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
	{
		game_data->DebugDrawGUI();
	}

	// ゲームオブジェクト
	if (GameObject::Instance game_object = GameObject::GetInstance(); game_object.Get())
	{
		game_object->DebugDrawGUI();
	}

	// FPS
	{
#ifdef _DEBUG
		if (ImGui::Begin("FPS"))
		{
			ImGui::InputFloat("FPS", &fps);
			ImGui::InputFloat("mspf", &mspf);
			ImGui::SliderInt("SyncInterval", &this->sync_interval, 0, 1);
		}
		ImGui::End();
#endif // DEBUG
	}
}

#endif // DEBUG