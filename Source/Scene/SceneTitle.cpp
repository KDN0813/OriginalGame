#include "SceneTitle.h"
#include "Scene/SceneLoading.h"
#include "Scene/SceneGame.h"
#include "Scene/SceneManager.h"
#include "Graphics/Graphics.h"
#include "Input\Input.h"
#ifdef _DEBUG
#include "Debug/ImGuiHelper.h"
#endif // DEBUG

#include "Component/SpriteComponent.h"
#include "Component\Transform2DComponent.h"

void SceneTitle::Initialize()
{
    // シェーダー作成
    {
        this->sprite_shader = std::make_unique<SpriteShader>();
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
				this->sprite_shader->AddSprite(sprite);
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
				param.display_pos = { 0.223f,0.1f };
				param.display_size = { 0.55f,0.35f };
				param.filename = "Data/Sprite/TitleLogo.png";
				auto sprite = sprite_logo->AddComponent<SpriteComponent>(param);

				this->sprite_shader->AddSprite(sprite);
			}
		}

		// タイトルロゴ
		{
			auto sprite_bg = object_manager.Create("Game Staet Text");
			SpriteComponent::SpriteParam param{};
			param.color = { 1.0f,1.0f, 1.0f, 1.0f };
			param.display_pos = { 0.254f,0.792f };
			param.display_size = { 0.5f,0.1f };
			param.filename = "Data/Sprite/TitleGameStartText.png";
			auto sprite = sprite_bg->AddComponent<SpriteComponent>(param);

			this->sprite_shader->AddSprite(sprite);
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

		// ボタンが押されたら
		if ((GamePad::BTN_A | GamePad::BTN_B | GamePad::BTN_X | GamePad::BTN_Y) & game_pad.GetButtonDown())
		{
			if (SceneManager::Instance scene_manager = SceneManager::GetInstance(); scene_manager.Get())
			{
				scene_manager->ChangeScene(new SceneLoading(new SceneGame()));
			}
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
	{
		this->sprite_shader->Render();
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
			this->sprite_shader->DrawDebugGUI();
		}
		ImGui::End();
	}
}

#endif // DEBUG