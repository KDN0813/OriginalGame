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
	effect_manager.Initialize();
#if defined(_DEBUG)
	scene_manager.ChangeScene(new SceneGame);
	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);  // �f�t�H���g�l���Đݒ�
#elif defined(RELEASE_DEBUG)
	scene_manager.ChangeScene(new SceneGame);
#else
	scene_manager.ChangeScene(new SceneTitle);
#endif // _DEBUG
}

Framework::~Framework()
{
	scene_manager.Clear();
	effect_manager.Finalize();
	ReleaseDC(hWnd, hDC);
}

void Framework::Update(float elapsed_time)
{
	input.Update();

#ifdef _DEBUG
	// �L�[���͂ɂ��f�o�b�O�p�@�\
	if (Input::Instance input = Input::GetInstance(); input.Get())
	{
		if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
		{
			const auto& game_pad = input->GetGamePad();

			// ImGui�\���E��\���{�^������
			if (GamePad::BTN_DEBUG_IMGUI & game_pad.GetButtonDown())
			{
				game_data->SetDrawImguiFlag(!game_data->GetDrawImguiFlag());
			}
			// �f�o�b�O�v���~�e�B�u�\���E��\���{�^������
			if (GamePad::BTN_DEBUG_PRIMITIVE & game_pad.GetButtonDown())
			{
				game_data->SetDrawDebugPrimitiveFlag(!game_data->GetDrawDebugPrimitiveFlag());
			}
			// �^�C�g���ɖ߂�{�^������
			if (GamePad::BTN_DEBUG_RETURN_TO_TITLE & game_pad.GetButtonDown())
			{
				// �������A���[�h��ʂȂ�J�ڂ��Ȃ�
				if (SceneManager::Instance scene_manager = SceneManager::GetInstance(); (scene_manager.Get()&& !game_data->GetIsLoading()))
				{
					scene_manager->ChangeScene(new SceneTitle);
				}
			}
			// �����Q�[���I���{�^��
			if (GamePad::BTN_DEBUG_FORCE_EXIT & game_pad.GetButtonDown())
			{
				game_data->CloseWindow();
			}
		}
	}

#endif // _DEBUG

	// �t���[�����[�g�v�Z
	game_timer.CalculateFrameStats();

	scene_manager.Update(elapsed_time);

	// �E�B���h�E�̏I������
	if (IsWindowClose())
	{
		PostMessage(hWnd, WM_CLOSE, 0, 0);
	}
}

void Framework::Render(float elapsed_time)
{
#ifdef _DEBUG
	if (GameData::Instance game_data = GameData::GetInstance(); (game_data.Get() && game_data->GetDrawImguiFlag()))
	{
		debug_manager.GetImGuiRenderer()->NewFrame();
	}
#endif // _DEBUG

	scene_manager.Render();

#ifdef _DEBUG
	if (GameData::Instance game_data = GameData::GetInstance(); (game_data.Get() && game_data->GetDrawImguiFlag()))
	{
		DrawDebugGUI();
		debug_manager.GetImGuiRenderer()->Render();
	}
#endif // _DEBUG

	UINT flags = (graphics.GetTearingSupported() && !this->fullscreen_mode && !this->sync_interval) ? DXGI_PRESENT_ALLOW_TEARING : 0;
	graphics.GetSwapChain()->Present(this->sync_interval, flags);
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
				if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
				{
					if (game_data->GetGameStatus() == GameData::GameStatus::GAME)
					{
						game_data->SetIsPause(!game_data->GetIsPause());
						this->scene_manager.GetPauseObject()->SetIsActive(game_data->GetIsPause());	// pause�p�I�u�W�F�N�g��(��)�A�N�e�B�u��
					}
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
		if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
		{
			// ���[�h���Ȃ�E�B���h�E����Ȃ�
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
	// �V�[��
	this->scene_manager.DrawDebugGUI();

	// �J����
	if (CameraManager::Instance camera_manager = CameraManager::GetInstance(); camera_manager.Get())
	{
		camera_manager->DrawDebugGUI();
	}

	// �I�[�f�B�I
	if (Audio::Instance audio = Audio::GetInstance(); audio.Get())
	{
		audio->DebugDrawGUI();
	}

	// �Q�[���f�[�^
	if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
	{
		game_data->DebugDrawGUI();
	}

	// �Q�[���I�u�W�F�N�g
	if (GameObject::Instance game_object = GameObject::GetInstance(); game_object.Get())
	{
		game_object->DebugDrawGUI();
	}

	// FPS
	{
		game_timer.DebugDrawGUI(this->sync_interval);
	}
}

#endif // DEBUG