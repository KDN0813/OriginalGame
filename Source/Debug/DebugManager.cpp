#include "DebugManager.h"

#include "Graphics/Graphics.h"
#include "Input\Input.h"
#include "System\GameData.h"
#include "Scene\SceneManager.h"

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

// キー入力によるデバッグ用機能
void DebugManager::HandleFunctionKeyDebug()
{
	Input::Instance input = Input::GetInstance();
	GameData::Instance game_data = GameData::GetInstance();
	const auto& game_pad = input->GetGamePad();

	// ImGui表示・非表示ボタン制御
	if (GamePad::BTN_DEBUG_IMGUI & game_pad.GetButtonDown())
	{
		game_data->SetDrawImguiFlag(!game_data->GetDrawImguiFlag());
	}
	// デバッグプリミティブ表示・非表示ボタン制御
	if (GamePad::BTN_DEBUG_PRIMITIVE & game_pad.GetButtonDown())
	{
		game_data->SetDrawDebugPrimitiveFlag(!game_data->GetDrawDebugPrimitiveFlag());
	}
	// タイトルに戻るボタン制御
	if (GamePad::BTN_DEBUG_RETURN_TO_TITLE & game_pad.GetButtonDown())
	{
		// ただじ、ロード画面なら遷移しない
		SceneManager::Instance scene_manager = SceneManager::GetInstance();
		scene_manager->ChangeScene(new SceneTitle);
	}
	// 強制ゲーム終了ボタン
	if (GamePad::BTN_DEBUG_FORCE_EXIT & game_pad.GetButtonDown())
	{
		game_data->CloseWindow();
	}
	// タイマーの停止
	if (GamePad::BTN_DEBUG_STOP_TIMER & game_pad.GetButtonDown())
	{
		game_data->SetIsStopTimer(!game_data->GetIsStopTimer());
	}
}

#endif // _DEBUG