#include "UIConstant.h"
#include "Object\Object.h"
#include "Shader\SpriteShader.h"

#include "Shader\SpriteShader.h"

#include "Component\TextNumberComponent.h"
#include "Component\SpriteComponent.h"
#include "Component\StateMachineComponent.h"
#include "Component\Transform2DComponent.h"
#include "Component\SpriteComponent.h"

#include "StateMachine\UIStateDerived.h"

const std::shared_ptr<Object>& UIConstant::CreateScoreUI(const std::shared_ptr<Object>& score_object)
{
	// transform
	{
		Transform2DComponent::Transform2DParam param{};
		param.local_position = { 0.86f,0.110f };
		param.local_scale = { 2.5f,1.5f };
		score_object->AddComponent<Transform2DComponent>(param);
	}

	// 子オブジェクト
	{
		// 背景
		{
			const auto bg_sprite = score_object->CreateChildObject("BG_Sprite");

			// transform
			{
				SpriteComponent::SpriteParam param{};
				param.display_size = { 0.144f,0.015f };
				param.color = { 1.0f,1.0f, 1.0f, 1.0f };
				param.center_type = Sprite::CENTER_TYPE::CENTER;
				const auto& sprite = bg_sprite->AddComponent<SpriteComponent>(param);
				if (SpriteShader::Instance sprite_shader = SpriteShader::GetInstance(); sprite_shader.Get())
				{
					sprite_shader->AddSprite(sprite);
				}
			}

			// transform
			{
				Transform2DComponent::Transform2DParam param{};
				param.local_position = { 0.0f,0.039f };
				bg_sprite->AddComponent<Transform2DComponent>(param);
			}
		}

		// テキスト
		{
			const auto text_sprite = score_object->CreateChildObject("Text");

			// TextNumberComponent
			{
				TextNumberComponent::TextParam param{};
				param.color = { 1.0f,0.0f,0.0f ,1.0f };
				param.center_type = Sprite::CENTER_TYPE::CENTER_RIGHT;
				// ファイルパス設定する
				param.font_name = "Data/Sprite/Numbers.png";
				auto text_number = text_sprite->AddComponent<TextNumberComponent>(param);

				if (SpriteShader::Instance sprite_shader = SpriteShader::GetInstance(); sprite_shader.Get())
				{
					sprite_shader->AddSprite(text_number);
				}
			}

			// transform
			{
				Transform2DComponent::Transform2DParam param{};
				param.local_position = { 0.005f,0.0f };
				text_sprite->AddComponent<Transform2DComponent>(param);
			}

			// 更新処理
			{
				auto state_machine = text_sprite->AddComponent<StateMachineComponent>();

				state_machine->RegisterState<ScoreUIDefaultState>();

				state_machine->SetDefaultState(ScoreUIDefaultState::STATE_NAME);
			}
		}
	}

	return score_object;
}

const std::shared_ptr<Object>& UIConstant::CreateGameTimerUI(const std::shared_ptr<Object>& sprite_shader)
{
	// transform
	{
		Transform2DComponent::Transform2DParam param{};
		param.local_position = { 0.5f,0.065f };
		param.local_scale = { 1.3f,1.3f };
		sprite_shader->AddComponent<Transform2DComponent>(param);
	}

	// 子オブジェクト
	{
		// 背景
		{
			const auto bg_sprite = sprite_shader->CreateChildObject("BG_Sprite");

			// transform
			{
				SpriteComponent::SpriteParam param{};
				param.display_size = { 0.1f,0.1f };
				param.color = { 1.0f,1.0f, 1.0f, 1.0f };
				param.center_type = Sprite::CENTER_TYPE::CENTER;
				const auto& sprite = bg_sprite->AddComponent<SpriteComponent>(param);
				if (SpriteShader::Instance sprite_shader = SpriteShader::GetInstance(); sprite_shader.Get())
				{
					sprite_shader->AddSprite(sprite);
				}
			}

			// transform
			{
				Transform2DComponent::Transform2DParam param{};
				bg_sprite->AddComponent<Transform2DComponent>(param);
			}
		}

		// テキスト
		{
			const auto& text_sprite = sprite_shader->CreateChildObject("Text");

			// TextNumberComponent
			{
				TextNumberComponent::TextParam param{};
				param.color = { 1.0f,0.0f,0.0f ,1.0f };
				param.center_type = Sprite::CENTER_TYPE::CENTER_RIGHT;
				// ファイルパス設定する
				param.font_name = "Data/Sprite/Numbers.png";
				auto text_number = text_sprite->AddComponent<TextNumberComponent>(param);

				if (SpriteShader::Instance sprite_shader = SpriteShader::GetInstance(); sprite_shader.Get())
				{
					sprite_shader->AddSprite(text_number);
				}
			}

			// transform
			{
				Transform2DComponent::Transform2DParam paam{};
				paam.local_position = { 0.045f,0.0f };
				text_sprite->AddComponent<Transform2DComponent>(paam);
			}

			// 更新処理
			{
				auto state_machine = text_sprite->AddComponent<StateMachineComponent>();

				state_machine->RegisterState<EndTimerUIDefaultState>();

				state_machine->SetDefaultState(EndTimerUIDefaultState::STATE_NAME);
			}
		}
	}

	return sprite_shader;
}

const std::shared_ptr<Object>& UIConstant::CreatePause(const std::shared_ptr<Object>& pause_object)
{
	// スプライト読み込み
	{
		SpriteComponent::SpriteParam param{};
		param.color = { 1.0f,1.0f, 1.0f, 1.0f };
		param.filename = "Data/Sprite/Pause.png";
		auto sprite = pause_object->AddComponent<SpriteComponent>(param);

		if (SpriteShader::Instance sprite_shader = SpriteShader::GetInstance(); sprite_shader.Get())
		{
			sprite_shader->AddSprite(sprite);
		}
	}

	return pause_object;
}
