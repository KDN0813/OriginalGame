#include "SceneResult.h"
#include "Graphics/Graphics.h"

// シェーダー
#include "Shader/SpriteShader.h"

#include "Scene/SceneManager.h"
#include "Scene/SceneTitle.h"

#include "Component/SpriteComponent.h"
#include "Component/Transform2DComponent.h"


void SceneResult::Initialize()
{
	// オブジェクト作成
	{
		// 背景
		{
			auto sprite_bg = object_manager.Create("Result Back Sprite");
			// スプライト
			{
				SpriteComponent::SpriteParam param{};
				param.filename = "Data/Sprite/ResultBack.png";
				auto sprite = sprite_bg->AddComponent<SpriteComponent>(param);
			}
			// transform
			{
				Transform2DComponent::Transform2DParam param{};
				sprite_bg->AddComponent<Transform2DComponent>(param);
			}
		}

		// スコアUI
		{
			auto result_text = object_manager.Create("Result Text");
			// スプライト
			{
				SpriteComponent::SpriteParam param{};
				param.filename = "Data/Sprite/ResultText.png";
				param.center_type = Sprite::CENTER_TYPE::CENTER;
				auto sprite = result_text->AddComponent<SpriteComponent>(param);
				sprite->AdjustDisplaySizeToSprite();
			}
			// transform
			{
				Transform2DComponent::Transform2DParam param{};
				param.local_scale = { 0.5f,0.5f };
				param.local_position = { 0.5f,0.2f };
				result_text->AddComponent<Transform2DComponent>(param);
			}
		}

		// リザルトテキスト
		{
			auto score_text = object_manager.Create("Score Text");
			// スプライト
			{
				SpriteComponent::SpriteParam param{};
				param.filename = "Data/Sprite/ScoreText.png";
				param.center_type = Sprite::CENTER_TYPE::CENTER;
				auto sprite = score_text->AddComponent<SpriteComponent>(param);
				sprite->AdjustDisplaySizeToSprite();
			}
			// transform
			{
				Transform2DComponent::Transform2DParam param{};
				param.local_position = { 0.44f,0.48f };
				param.local_scale = { 0.45f,0.45f };
				score_text->AddComponent<Transform2DComponent>(param);
			}
		}

		// リザルトUI
		{
			auto resultUI = object_manager.Create("Result UI");
			// スプライト
			{
				SpriteComponent::SpriteParam param{};
				param.filename = "Data/Sprite/ResultUI.png";
				param.center_type = Sprite::CENTER_TYPE::CENTER;
				auto sprite = resultUI->AddComponent<SpriteComponent>(param);
				sprite->AdjustDisplaySizeToSprite();
			}
			// transform
			{
				Transform2DComponent::Transform2DParam param{};
				param.local_position = { 0.555f,0.8f };
				param.local_scale = { 0.45f,0.45f };
				resultUI->AddComponent<Transform2DComponent>(param);
			}
		}
	}
}

void SceneResult::Finalize()
{
}

void SceneResult::Update(float elapsed_time)
{
	this->object_manager.Update(elapsed_time);
}

void SceneResult::Render()
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

void SceneResult::DebugDrawGUI()
{
	this->object_manager.DrawDebugGUI();
}
