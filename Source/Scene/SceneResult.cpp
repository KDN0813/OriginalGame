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
		// �w�i
		{
			auto sprite_bg = object_manager.Create("Result Back Sprite");
			// �X�v���C�g
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

		// �X�R�AUI
		{
			auto result_text = object_manager.Create("Result Text");
			// �X�v���C�g
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

		// ���U���g�e�L�X�g
		{
			auto score_text = object_manager.Create("Score Text");
			// �X�v���C�g
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

		// ���U���gUI
		{
			auto resultUI = object_manager.Create("Result UI");
			// �X�v���C�g
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

void SceneResult::DebugDrawGUI()
{
	this->object_manager.DrawDebugGUI();
}
