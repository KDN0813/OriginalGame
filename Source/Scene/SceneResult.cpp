#include "SceneResult.h"
#include "Graphics/Graphics.h"

// �V�F�[�_�[
#include "Shader/SpriteShader.h"

#include "Scene/SceneManager.h"
#include "Scene/SceneTitle.h"

#include "Component/SpriteComponent.h"
#include "Component/Transform2DComponent.h"


void SceneResult::Initialize()
{
	// �I�u�W�F�N�g�쐬
	{
		// �^�C�g���w�i
		{
			auto sprite_bg = object_manager.Create("Title Back Sprite");
			// �X�v���C�g
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

	// �X�y�[�X�L�[�ŃQ�[����ʂɑJ��(��)
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::Instance scene_manager = SceneManager::GetInstance();
		if (!scene_manager.Get()) return;
		scene_manager->ChangeScene(new SceneTitle());
	}
}

void SceneResult::Render()
{
	// �`�揀��
	Graphics::Instance graphics = Graphics::GetInstance();
	if (!graphics.Get()) return;
	graphics->PrepareRenderTargets();

	// 2D�X�v���C�g�`��
	if (SpriteShader::Instance sprite_shader = SpriteShader::GetInstance(); sprite_shader.Get())
	{
		sprite_shader->Render();
	}
}
