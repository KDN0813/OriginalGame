#include "UIConstant.h"
#include "Object\Object.h"
#include "Object\Constant\PlayerConstant.h"
#include "Shader\SpriteShader.h"

#include "Shader\SpriteShader.h"

#include "Component\TextNumberComponent.h"
#include "Component\SpriteComponent.h"
#include "Component\StateMachineComponent.h"
#include "Component\Transform2DComponent.h"
#include "Component\FadeControllerComponent.h"
#include "Component\SpecialGageComponent.h"

#include "StateMachine\UIStateDerived.h"

const MyHash UIConstant::SPIN_SPECIAL_GAGE_OBJECT_NAME = MyHash("SpecialGage");

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

	// 子オブジェクト
	{
		// ポーズUI
		{
			const auto& pause_ui = pause_object->CreateChildObject("Pause UI");
			// スプライト読み込み
			{
				SpriteComponent::SpriteParam param{};
				param.color = { 1.0f,1.0f, 1.0f, 1.0f };
				param.filename = "Data/Sprite/PauseUI.png";
				param.center_type = Sprite::CENTER_TYPE::CENTER;
				param.draw_priority = PRIORITY::LOWEST;
				auto sprite = pause_ui->AddComponent<SpriteComponent>(param);
			}

			// transform
			{
				Transform2DComponent::Transform2DParam param{};
				param.local_position = { 0.6f,0.64f };
				param.local_scale = { 0.75f,0.75f };
				pause_ui->AddComponent<Transform2DComponent>(param);
			}
		}
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
						paam.local_position = { 0.04f,-0.49f };
						paam.local_scale = { 0.9f,0.9f };
						bar_object->AddComponent<Transform2DComponent>(paam);
					}

					// スプライト読み込み
					{
						SpriteComponent::SpriteParam param{};
						param.color = PLAYER_HP_BAR_COLOR;
						param.draw_priority = PRIORITY::DEFAULT;
						param.center_type = Sprite::CENTER_TYPE::CENTER_LEFT;
						bar_object->AddComponent<SpriteComponent>(param);
					}

					// 更新処理
					{
						auto state_machine = bar_object->AddComponent<StateMachineComponent>();

						state_machine->RegisterState<PlayerHPBarUIState>();
						
						state_machine->SetDefaultState(PlayerHPBarUIState::STATE_NAME);
					}
				}
			}
		}
	}

	return hp_ber;
}

const std::shared_ptr<Object>& UIConstant::CreatePlayerSpecialBar(const std::shared_ptr<Object>& special_bar)
{
	// transform
	{
		Transform2DComponent::Transform2DParam paam{};
		paam.local_position = { 0.0f, 0.840f };
		paam.local_scale = { 0.2f,0.15f };
		special_bar->AddComponent<Transform2DComponent>(paam);
	}

	// 子オブジェクト
	{
		// 背景
		{
			const auto bg_object = special_bar->CreateChildObject("SpriteBG");

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
				paam.local_scale = { 1.0f,0.3f };
				bg_object->AddComponent<Transform2DComponent>(paam);
			}

			// 子オブジェクト
			{
				// 体力ゲージ(フレーム)
				{
					const auto bg_frame_object = bg_object->CreateChildObject("HpFrameBer");

					// transform
					{
						Transform2DComponent::Transform2DParam paam{};
						paam.local_position = { 0.04f,-0.49f };
						paam.local_scale = { 0.9f,0.75f };
						bg_frame_object->AddComponent<Transform2DComponent>(paam);
					}

					// スプライト読み込み
					{
						SpriteComponent::SpriteParam param{};
						param.color = DEFAULT_SPIN_ATTACK_DESCRIPTION_UI_COLOR;
						param.draw_priority = PRIORITY::DEFAULT;
						param.center_type = Sprite::CENTER_TYPE::CENTER_LEFT;
						bg_frame_object->AddComponent<SpriteComponent>(param);
					}

					// 子オブジェクト
					{
						// スペシャルポイント
						{
							const auto bar_object = bg_frame_object->CreateChildObject("HpBer");

							// transform
							{
								Transform2DComponent::Transform2DParam paam{};
								bar_object->AddComponent<Transform2DComponent>(paam);
							}

							// スプライト読み込み
							{
								SpriteComponent::SpriteParam param{};
								param.color = { 1.0f,1.0f, 0.0f, 1.0f };
								param.draw_priority = PRIORITY::DEFAULT;
								param.center_type = Sprite::CENTER_TYPE::CENTER_LEFT;
								bar_object->AddComponent<SpriteComponent>(param);
							}

							// 更新処理
							{
								const auto& special_gage = bar_object->AddComponent<SpecialGageComponent>();
							}

							// 子オブジェクト
							{
								DirectX::XMFLOAT2 offset_pos{ -0.02f,-1.190f };
								const float offsetX = 0.37f;
								for (int i = 0; i < PlayerConstant::SPECIAL_GAGE_MAX; ++i)
								{
									CreatePlayerSpecialGage(bar_object->CreateChildObject((SPIN_SPECIAL_GAGE_OBJECT_NAME.GetString() + std::to_string(i)).c_str()), offset_pos);
									offset_pos.x += offsetX;
								}
							}
						}
					}
				}
			}
		}
	}

	return special_bar;
}

const std::shared_ptr<Object>& UIConstant::CreatePlayerSpecialGage(const std::shared_ptr<Object>& object, DirectX::XMFLOAT2 pos)
{
	// transform
	{
		Transform2DComponent::Transform2DParam param{};
		param.local_position = pos;
		param.local_scale = { 0.8f,3.0f };
		object->AddComponent<Transform2DComponent>(param);
	}

	// スプライト読み込み
	{
		SpriteComponent::SpriteParam param{};
		param.color = { 1.0f,1.0f, 0.0f, 1.0f };
		param.filename = "Data/Sprite/SpecialGage.png";
		param.draw_priority = PRIORITY::DEFAULT;
		param.center_type = Sprite::CENTER_TYPE::CENTER_LEFT;
		const auto& sprite = object->AddComponent<SpriteComponent>(param);
		sprite->AdjustDisplaySizeToSprite();
	}

	return object;
}

const std::shared_ptr<Object>& UIConstant::CreateDescriptionUI(const std::shared_ptr<Object>& descriptionUI)
{
	// transform
	{
		Transform2DComponent::Transform2DParam paam{};
		paam.local_position = { 0.0f, 0.55f };
		descriptionUI->AddComponent<Transform2DComponent>(paam);
	}

	// 子オブジェクト
	{
		// 説明UI1
		{
			// DescriptionUI1
			{
				const auto description_UI1 = descriptionUI->CreateChildObject("DescriptionUI1");

				// transform
				{
					Transform2DComponent::Transform2DParam paam{};
					paam.local_scale = { 0.2f,0.2f };
					description_UI1->AddComponent<Transform2DComponent>(paam);
				}

				// スプライト読み込み
				{
					SpriteComponent::SpriteParam param{};
					param.color = { 1.0f,1.0f, 1.0f, 1.0f };
					param.filename = "Data/Sprite/DescriptionUI1.png";
					param.draw_priority = PRIORITY::DEFAULT;
					param.center_type = Sprite::CENTER_TYPE::CENTER_LEFT;
					const auto& sprite = description_UI1->AddComponent<SpriteComponent>(param);
					sprite->AdjustDisplaySizeToSprite();
				}
			}

			// DescriptionUI2
			{
				const auto description_UI2 = descriptionUI->CreateChildObject("DescriptionUI2");

				// transform
				{
					Transform2DComponent::Transform2DParam param{};
					param.local_position = { 0.0f,-0.084f };
					param.local_scale = { 0.2f,0.2f };
					description_UI2->AddComponent<Transform2DComponent>(param);
				}

				// スプライト読み込み
				{
					SpriteComponent::SpriteParam param{};
					param.color = DEFAULT_SPIN_ATTACK_DESCRIPTION_UI_COLOR;
					param.filename = "Data/Sprite/DescriptionUI2.png";
					param.draw_priority = PRIORITY::DEFAULT;
					param.center_type = Sprite::CENTER_TYPE::CENTER_LEFT;
					const auto& sprite = description_UI2->AddComponent<SpriteComponent>(param);

					sprite->AdjustDisplaySizeToSprite();
				}

				// 更新
				{
					const auto& state_machine = description_UI2->AddComponent<StateMachineComponent>();

					state_machine->RegisterState<SpinAttackDescriptionUIState>();
					state_machine->SetDefaultState(SpinAttackDescriptionUIState::STATE_NAME);
				}
			}
		}
	}

	return descriptionUI;
}
