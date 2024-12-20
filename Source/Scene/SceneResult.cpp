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
		// タイトル背景
		{
			auto sprite_bg = object_manager.Create("Title Back Sprite");
			// スプライト
			{
				SpriteComponent::SpriteParam param{};
				param.filename = "Data/Debug/Sprite/Title.png";
				param.color = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
				auto sprite = sprite_bg->AddComponent<SpriteComponent>(param);
				if (SpriteShader::Instance sprite_shader = SpriteShader::GetInstance(); sprite_shader.Get())
				{
					sprite_shader->AddSprite(sprite);
				}
			}
			// transform
			{
				Transform2DComponent::Transform2DParam param{};
				sprite_bg->AddComponent<Transform2DComponent>(param);
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

	// スペースキーでゲーム画面に遷移(仮)
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::Instance scene_manager = SceneManager::GetInstance();
		if (!scene_manager.Get()) return;
		scene_manager->ChangeScene(new SceneTitle());
	}
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
