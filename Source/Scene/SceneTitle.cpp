#include "SceneTitle.h"
#include "Scene/SceneLoading.h"
#include "Scene/SceneGame.h"
#include "Scene/SceneManager.h"
#include "Graphics/Graphics.h"
#include "Input\Input.h"
#include "System\GameData.h"
#include "Audio\Audio.h"
#ifdef _DEBUG
#include "Debug/ImGuiHelper.h"
#endif // DEBUG

#include "Shader/SpriteShader.h"

#include "Component/SpriteComponent.h"
#include "Component\Transform2DComponent.h"

void SceneTitle::Initialize()
{
	// ゲーム状態を設定
	{
		GameData::Instance game_data = GameData::GetInstance();
		game_data->SetGameStatus(GameData::GameStatus::TITLE);
	}

	// オブジェクト作成
	{
		// タイトル背景
		{
			auto sprite_bg = object_manager.Create("Title Back");

			// スプライト読み込み
			{
				BaseSpriteComponent::SpriteParam sprite_param{};
				sprite_param.color = { 1.0f,1.0f, 1.0f, 1.0f };
				sprite_param.filename = "Data/Sprite/TitleBack.png";
				auto sprite = sprite_bg->AddComponent<SpriteComponent>(sprite_param);
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
				BaseSpriteComponent::SpriteParam sprite_param{};
				sprite_param.display_size = { 0.55f,0.35f };
				sprite_param.color = { 1.0f,1.0f, 1.0f, 1.0f };
				sprite_param.filename = "Data/Sprite/TitleLogo.png";
				sprite_param.center_type = Sprite::CENTER_TYPE::CENTER;
				auto sprite = sprite_logo->AddComponent<SpriteComponent>(sprite_param);
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
				BaseSpriteComponent::SpriteParam sprite_param{};
				sprite_param.color = { 1.0f,1.0f, 1.0f, 1.0f };
				sprite_param.center_type = Sprite::CENTER_TYPE::CENTER;
				sprite_param.filename = "Data/Sprite/TitleGameStartText.png";
				auto sprite = sprite_bg->AddComponent<SpriteComponent>(sprite_param);
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
				BaseSpriteComponent::SpriteParam sprite_param{};
				sprite_param.color = { 1.0f,1.0f, 1.0f, 1.0f };
				sprite_param.center_type = Sprite::CENTER_TYPE::CENTER;
				sprite_param.filename = "Data/Sprite/TitleGameEndText.png";
				auto sprite = sprite_bg->AddComponent<SpriteComponent>(sprite_param);
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

	// オーディオ再生
	{
		Audio::Instance audio = Audio::GetInstance();
		AudioParam param{};
		param.volume = 0.5f;
		param.loop = true;
		param.filename = "Data/Audio/BGM.wav";
		audio->Play(param);
	}
}

void SceneTitle::Finalize()
{
	// オーディオのリセット
	{
		Audio::Instance audio = Audio::GetInstance();
		audio->AllClear();
	}
}

void SceneTitle::Update(float elapsed_time)
{
	object_manager.Update(elapsed_time);

	// シーン遷移
	{
		Input::Instance input = Input::GetInstance();
		const auto& game_pad = input->GetGamePad();

		// Xボタンが押されたら
		if (GamePad::BTN_X & game_pad.GetButtonDown())
		{
			SceneManager::Instance scene_manager = SceneManager::GetInstance();
			scene_manager->ChangeScene(new SceneLoading(new SceneGame()));
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
	graphics->PrepareRenderTargets();

	// 2Dスプライト描画
	{
		SpriteShader::Instance sprite_shader = SpriteShader::GetInstance();
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
			SpriteShader::Instance sprite_shader = SpriteShader::GetInstance();
			sprite_shader->DrawDebugGUI();
		}
		ImGui::End();
	}
}

#endif // DEBUG