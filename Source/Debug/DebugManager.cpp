#include "DebugManager.h"

#include "Graphics/Graphics.h"
#include "Input\Input.h"
#include "System\GameData.h"
#include "Scene\SceneManager.h"
#include "System\GameTimer.h"

#include "Scene\/SceneTitle.h"

DebugManager::DebugManager(HWND hWnd)
    :Singleton(this)
{
    Graphics::Instance graphics = Graphics::GetInstance();
    ID3D11Device*  device = graphics->GetDevice();

    this->imGui_renderer = std::make_unique<ImGuiRenderer>(hWnd, device);
    this->debug_primitive_renderer = std::make_unique<DebugPrimitiveRenderer>(device);
}

#ifdef _DEBUG

// �L�[���͂ɂ��f�o�b�O�p�@�\
void DebugManager::HandleFunctionKeyDebug()
{
	Input::Instance input = Input::GetInstance();
	GameData::Instance game_data = GameData::GetInstance();
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
		SceneManager::Instance scene_manager = SceneManager::GetInstance();
		scene_manager->ChangeScene(new SceneTitle);
	}
	// �����Q�[���I���{�^��
	if (GamePad::BTN_DEBUG_FORCE_EXIT & game_pad.GetButtonDown())
	{
		game_data->CloseWindow();
	}
	// �^�C�}�[�̒�~
	if (GamePad::BTN_DEBUG_STOP_TIMER & game_pad.GetButtonDown())
	{
		game_data->SetIsStopTimer(!game_data->GetIsStopTimer());
	}
	// �Q�[���X�s�[�h��0�i��~�j��1�i�ʏ�j�Ő؂�ւ���
	if (GamePad::BTN_DEBUG_GAME_SPEED_TOGGLE & game_pad.GetButtonDown())
	{
		GameTimer::Instance game_timer = GameTimer::GetInstance();
		game_timer->SetGameSpeed(game_timer->GetGameSpeed() <= 0.0f ? 1.0f : 0.0f);
	}
}

#endif // _DEBUG