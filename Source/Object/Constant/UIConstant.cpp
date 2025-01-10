#include "UIConstant.h"
#include "Object\Object.h"
#include "Shader\SpriteShader.h"

#include "Shader\SpriteShader.h"

#include "Component\TextNumberComponent.h"
#include "Component\SpriteComponent.h"
#include "Component\StateMachineComponent.h"
#include "Component\Transform2DComponent.h"
#include "Component\FadeControllerComponent.h"

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

			// 背景画像
			{
				SpriteComponent::SpriteParam param{};
				param.display_size = { 0.144f,0.015f };
				param.color = { 1.0f,1.0f, 1.0f, 1.0f };
				param.center_type = Sprite::CENTER_TYPE::CENTER;
				const auto& sprite = bg_sprite->AddComponent<SpriteComponent>(param);
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
			}

			// transform
			{
				Transform2DComponent::Transform2DParam param{};
				param.local_position = { 0.055f,0.0f };
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
		param.local_position = { 0.1f,0.09f };
		param.local_scale = { 2.0f,2.0f };
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
				param.color = { 1.0f,1.0f, 1.0f, 0.7f };
				param.center_type = Sprite::CENTER_TYPE::CENTER;
				const auto& sprite = bg_sprite->AddComponent<SpriteComponent>(param);
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
				param.color = { 1.0f,0.0f,0.0f ,0.7f };
				param.center_type = Sprite::CENTER_TYPE::CENTER_RIGHT;
				// ファイルパス設定する
				param.font_name = "Data/Sprite/Numbers.png";
				auto text_number = text_sprite->AddComponent<TextNumberComponent>(param);
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
	pause_object->SetIsActive(false);

	// スプライト読み込み
	{
		SpriteComponent::SpriteParam param{};
		param.color = { 1.0f,1.0f, 1.0f, 1.0f };
		param.filename = "Data/Sprite/Pause.png";
		param.draw_priority = PRIORITY::LOWEST;
		auto sprite = pause_object->AddComponent<SpriteComponent>(param);
	}

	// transform
	{
		Transform2DComponent::Transform2DParam paam{};
		pause_object->AddComponent<Transform2DComponent>(paam);
	}

	return pause_object;
}

const std::shared_ptr<Object>& UIConstant::CreateFadeObject(const std::shared_ptr<Object>& fade_object)
{
	// スプライト読み込み
	{
		SpriteComponent::SpriteParam param{};
		param.color = { 0.0f,0.0f, 0.0f, 0.0f };
		param.draw_priority = PRIORITY::LOWEST;
		auto sprite = fade_object->AddComponent<SpriteComponent>(param);
	}

	// transform
	{
		Transform2DComponent::Transform2DParam paam{};
		fade_object->AddComponent<Transform2DComponent>(paam);
	}

	// FadeControllerComponent
	{
		FadeControllerComponent::FadeControllerParam param{};
		const auto fade_controller = fade_object->AddComponent<FadeControllerComponent>(param);
		fade_controller->SetIsActive(false);
	}

	return fade_object;
}

const std::shared_ptr<Object>& UIConstant::CreatePlayerHpBarUI(const std::shared_ptr<Object>& hp_ber)
{
	// transform
	{
		Transform2DComponent::Transform2DParam paam{};
		paam.local_position = { 0.0f, 1.0f };
		paam.local_scale = { 0.2f,0.15f };
		hp_ber->AddComponent<Transform2DComponent>(paam);
	}

	// 子オブジェクト
	{
		// 背景
		{
			const auto bg_object = hp_ber->CreateChildObject("SpriteBG");

			// スプライト読み込み
			{
				SpriteComponent::SpriteParam param{};
				param.color = { 0.0f,0.0f, 0.0f, 1.0f };
				param.draw_priority = PRIORITY::DEFAULT;
				param.center_type = Sprite::CENTER_TYPE::BOTTOM_LEFT;
				bg_object->AddComponent<SpriteComponent>(param);
			}

			// transform
			{
				Transform2DComponent::Transform2DParam paam{};
				bg_object->AddComponent<Transform2DComponent>(paam);
			}

			// 子オブジェクト
			{
				// 体力ゲージ
				{
					const auto bar_object = bg_object->CreateChildObject("HpBer");

					// transform
					{
						Transform2DComponent::Transform2DParam paam{};
						bar_object->AddComponent<Transform2DComponent>(paam);
					}

					// スプライト読み込み
					{
						SpriteComponent::SpriteParam param{};
						param.color = { 0.0f,1.0f, 0.0f, 1.0f };
						param.draw_priority = PRIORITY::DEFAULT;
						param.center_type = Sprite::CENTER_TYPE::BOTTOM_LEFT;
						bar_object->AddComponent<SpriteComponent>(param);
					}

					// 更新処理
					//{
					//	auto state_machine = hp_ber->AddComponent<StateMachineComponent>();

					//	state_machine->RegisterState<EndTimerUIDefaultState>();

					//	state_machine->SetDefaultState(EndTimerUIDefaultState::STATE_NAME);
					//}
				}
			}
		}
	}

	return hp_ber;
}
