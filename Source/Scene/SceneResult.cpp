#include "SceneResult.h"
#include "Graphics/Graphics.h"

#include "Scene/SceneManager.h"
#include "Scene/SceneTitle.h"

#include "Component/SpriteComponent.h"

void SceneResult::Initialize()
{
	// シェーダー作成
	{
		this->sprite_shader = std::make_unique<SpriteShader>();
	}

	// オブジェクト作成
	{
		// タイトル背景
		{
			auto sprite_bg = object_manager.Create();
			sprite_bg->SetName("Title Back Sprite");
			SpriteComponent::SpriteParam param{};
			param.filename = "Data/Debug/Sprite/Title.png";
			param.color = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
			auto sprite = sprite_bg->AddComponent<SpriteComponent>(param);

			this->sprite_shader->AddSprite(sprite);
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
	{
		this->sprite_shader->Render();
	}
}
