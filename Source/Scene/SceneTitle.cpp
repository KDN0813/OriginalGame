#include "SceneTitle.h"
#include "Scene/SceneLoading.h"
#include "Scene/SceneGame.h"
#include "Scene/SceneManager.h"
#include "Graphics/Graphics.h"
#include "Input\Input.h"
#include "System\GameData.h"
#ifdef _DEBUG
#include "Debug/ImGuiHelper.h"
#endif // DEBUG

#include "Shader/SpriteShader.h"

#include "Component/SpriteComponent.h"
#include "Component\Transform2DComponent.h"

void SceneTitle::Initialize()
{
	// ゲーム状態を設定
	if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
	{
		game_data->SetGameStatus(GameData::GameStatus::TITLE);
	}

	// オブジェクト作成
	{
		// タイトル背景
		{
			auto sprite_bg = object_manager.Create("Title Back");

			// スプライト読み込み
			{
				SpriteComponent::SpriteParam param{};
				param.color = { 1.0f,1.0f, 1.0f, 1.0f };
				param.filename = "Data/Sprite/TitleBack.png";
				auto sprite = sprite_bg->AddComponent<SpriteComponent>(param);
			}

			// transform
			{
				Transform2DComponent::Transform2DParam paam{};
				sprite_bg->AddComponent<Transform2DComponent>(paam);
			}
		}

		// タイトルロゴ
		{
			auto sprite_logo = object_manager.Create();
			sprite_logo->SetName("Title Logo");

			// スプライト読み込み
			{
				SpriteComponent::SpriteParam param{};
				param.color = { 1.0f,1.0f, 1.0f, 1.0f };
				param.display_size = { 0.55f,0.35f };
				param.filename = "Data/Sprite/TitleLogo.png";
				param.center_type = Sprite::CENTER_TYPE::CENTER;
				auto sprite = sprite_logo->AddComponent<SpriteComponent>(param);
				sprite->AdjustDisplaySizeToSprite();
			}

			// transform
			{
				Transform2DComponent::Transform2DParam param{};
				param.local_position = { 0.5f,0.2f };
				param.local_scale = { 1.5f,1.5f };
				sprite_logo->AddComponent<Transform2DComponent>(param);
			}
		}

		// ゲームスタートテキスト
		{
			auto sprite_bg = object_manager.Create("Game Staet Text");
			{
				SpriteComponent::SpriteParam param{};
				param.color = { 1.0f,1.0f, 1.0f, 1.0f };
				param.center_type = Sprite::CENTER_TYPE::CENTER;
				param.filename = "Data/Sprite/TitleGameStartText.png";
				auto sprite = sprite_bg->AddComponent<SpriteComponent>(param);
				sprite->AdjustDisplaySizeToSprite();

			}

			// transform
			{
				Transform2DComponent::Transform2DParam param{};
				param.local_position = { 0.5f,0.630f };
				param.local_scale = { 0.4f,0.4f };
				sprite_bg->AddComponent<Transform2DComponent>(param);
			}
		}

		// ゲーム終了テキスト
		{
			auto sprite_bg = object_manager.Create("Game Staet End");
			{
				SpriteComponent::SpriteParam param{};
				param.color = { 1.0f,1.0f, 1.0f, 1.0f };
				param.center_type = Sprite::CENTER_TYPE::CENTER;
				param.filename = "Data/Sprite/TitleGameEndText.png";
				auto sprite = sprite_bg->AddComponent<SpriteComponent>(param);
				sprite->AdjustDisplaySizeToSprite();
			}

			// transform
			{
				Transform2DComponent::Transform2DParam param{};
				param.local_position = { 0.5f,0.87f };
				param.local_scale = { 0.4f,0.4f };
				sprite_bg->AddComponent<Transform2DComponent>(param);
			}
		}
	}
}

void SceneTitle::Finalize()
{
}

void SceneTitle::Update(float elapsed_time)
{
	object_manager.Update(elapsed_time);

	// スペースキーでゲーム画面に遷移(仮)
	if (Input::Instance input = Input::GetInstance(); input.Get())
	{
		const auto& game_pad = input->GetGamePad();

		// Xボタンが押されたら
		if (GamePad::BTN_X & game_pad.GetButtonDown())
		{
			if (SceneManager::Instance scene_manager = SceneManager::GetInstance(); scene_manager.Get())
			{
				scene_manager->ChangeScene(new SceneLoading(new SceneGame()));
			}
		}
		// [Yボタン/ESCキー] が押されたら
		else if((GamePad::BTN_Y | GamePad::BTN_EXIT) & game_pad.GetButtonDown())
		{
			GameData::Instance game_data = GameData::GetInstance();
			// ウィンドウを終了させる
			game_data->CloseWindow();
		}
	}
}

void SceneTitle::Render()
{
	// 描画準備
	Graphics::Instance graphics = Graphics::GetInstance();
	if (!graphics.Get()) return;
	graphics->PrepareRenderTargets();

	// 2Dスプライト描画
	if (SpriteShader::Instance sprite_shader = SpriteShader::GetInstance(); sprite_shader.Get())
	{
		sprite_shader->Render();
	}
}

void SceneTitle::ReStart()
{
	this->object_manager.ReStart();
}

#ifdef _DEBUG

void SceneTitle::DebugDrawGUI()
{
	// Object
	this->object_manager.DrawDebugGUI();

	// シェーダー
	{
		if (ImGui::Begin("Sahder"))
		{
			if (SpriteShader::Instance sprite_shader = SpriteShader::GetInstance(); sprite_shader.Get())
			{
				sprite_shader->DrawDebugGUI();
			}
		}
		ImGui::End();
	}
}

#endif // DEBUG