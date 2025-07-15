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
#include "Component\ChainScoreCounterComponent.h"
#include "Component\ChainScoreUIPopAnimationComponent.h"
#include "Component\SpriteScalerComponent.h"
#include "Component\SpriteMoverComponent.h"
#include "Component\ChainScoreUIMoveAnimationComponent.h"
#include "Component\ScoreUIValueAnimatorComponent.h"
#include "Component\TextNumberValueInterpolatorComponent.h"

#include "StateMachine\UIStateDerived.h"

const MyHash UIConstant::SPIN_SPECIAL_GAGE_OBJECT_NAME = MyHash("SpecialGage");

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
				BaseSpriteComponent::SpriteParam sprite_param{};
				sprite_param.display_size = { 0.1f,0.1f };
				sprite_param.color = { 1.0f,1.0f, 1.0f, 0.7f };
				sprite_param.center_type = Sprite::CENTER_TYPE::CENTER;
				const auto& sprite = bg_sprite->AddComponent<SpriteComponent>(sprite_param);
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
				BaseSpriteComponent::SpriteParam sprite_param{};
				sprite_param.color = { 1.0f,0.0f,0.0f ,0.7f };
				sprite_param.center_type = Sprite::CENTER_TYPE::CENTER_RIGHT;
				// ファイルパス設定する
				sprite_param.filename = "Data/Sprite/Numbers.png";
				auto text_number = text_sprite->AddComponent<TextNumberComponent>(sprite_param);
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

				state_machine->RegisterState<EndTimerUIDefaultState>("UIState");

				state_machine->SetDefaultState("UIState");
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
		BaseSpriteComponent::SpriteParam sprite_param{};
		sprite_param.color = { 1.0f,1.0f, 1.0f, 1.0f };
		sprite_param.filename = "Data/Sprite/Pause.png";
		sprite_param.draw_priority = PRIORITY::DEFAULT;
		auto sprite = pause_object->AddComponent<SpriteComponent>(sprite_param);
	}

	// transform
	{
		Transform2DComponent::Transform2DParam param{};
		pause_object->AddComponent<Transform2DComponent>(param);
	}

	// 子オブジェクト
	{
		// ポーズUI
		{
			const auto& pause_ui = pause_object->CreateChildObject("Pause UI");
			// スプライト読み込み
			{
				BaseSpriteComponent::SpriteParam sprite_param{};
				sprite_param.color = { 1.0f,1.0f, 1.0f, 1.0f };
				sprite_param.filename = "Data/Sprite/PauseUI.png";
				sprite_param.center_type = Sprite::CENTER_TYPE::CENTER;
				sprite_param.draw_priority = PRIORITY::LOW;
				auto sprite = pause_ui->AddComponent<SpriteComponent>(sprite_param);
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
		BaseSpriteComponent::SpriteParam sprite_param{};
		sprite_param.color = { 0.0f,0.0f, 0.0f, 0.0f };
		sprite_param.draw_priority = PRIORITY::LOWEST;
		auto sprite = fade_object->AddComponent<SpriteComponent>(sprite_param);
	}

	// transform
	{
		Transform2DComponent::Transform2DParam param{};
		fade_object->AddComponent<Transform2DComponent>(param);
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
				BaseSpriteComponent::SpriteParam sprite_param{};
				sprite_param.color = { 0.0f,0.0f, 0.0f, 1.0f };
				sprite_param.draw_priority = PRIORITY::DEFAULT;
				sprite_param.center_type = Sprite::CENTER_TYPE::BOTTOM_LEFT;
				bg_object->AddComponent<SpriteComponent>(sprite_param);
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
						BaseSpriteComponent::SpriteParam sprite_param{};
						sprite_param.color = PLAYER_HP_BAR_COLOR;
						sprite_param.draw_priority = PRIORITY::DEFAULT;
						sprite_param.center_type = Sprite::CENTER_TYPE::CENTER_LEFT;
						bar_object->AddComponent<SpriteComponent>(sprite_param);
					}

					// 更新処理
					{
						auto state_machine = bar_object->AddComponent<StateMachineComponent>();

						state_machine->RegisterState<PlayerHPBarUIState>("UIState");
						
						state_machine->SetDefaultState("UIState");
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
				BaseSpriteComponent::SpriteParam sprite_param{};
				sprite_param.color = { 0.0f,0.0f, 0.0f, 1.0f };
				sprite_param.draw_priority = PRIORITY::DEFAULT;
				sprite_param.center_type = Sprite::CENTER_TYPE::BOTTOM_LEFT;
				bg_object->AddComponent<SpriteComponent>(sprite_param);
			}

			// transform
			{
				Transform2DComponent::Transform2DParam paam{};
				paam.local_scale = { 1.0f,0.3f };
				bg_object->AddComponent<Transform2DComponent>(paam);
			}

			// 子オブジェクト
			{
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
						BaseSpriteComponent::SpriteParam sprite_param{};
						sprite_param.color = DEFAULT_SPIN_ATTACK_DESCRIPTION_UI_COLOR;
						sprite_param.draw_priority = PRIORITY::DEFAULT;
						sprite_param.center_type = Sprite::CENTER_TYPE::CENTER_LEFT;
						bg_frame_object->AddComponent<SpriteComponent>(sprite_param);
					}

					// 更新
					{
						const auto& state_machine = bg_frame_object->AddComponent<StateMachineComponent>();
					
						state_machine->RegisterState<PlayerSpecialPointFrameUIState>("UIState");
						state_machine->SetDefaultState("UIState");
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
								BaseSpriteComponent::SpriteParam sprite_param{};
								sprite_param.color = PLAYER_SPECIAL_BAR_COLOR;
								sprite_param.draw_priority = PRIORITY::DEFAULT;
								sprite_param.center_type = Sprite::CENTER_TYPE::CENTER_LEFT;
								bar_object->AddComponent<SpriteComponent>(sprite_param);
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
		BaseSpriteComponent::SpriteParam sprite_param{};
		sprite_param.color = { 1.0f,1.0f, 0.0f, 1.0f };
		sprite_param.filename = "Data/Sprite/SpecialGage.png";
		sprite_param.draw_priority = PRIORITY::DEFAULT;
		sprite_param.center_type = Sprite::CENTER_TYPE::CENTER_LEFT;
		const auto& sprite = object->AddComponent<SpriteComponent>(sprite_param);
		sprite->AdjustDisplaySizeToSprite();
	}

	object->SetIsActive(false);

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
					BaseSpriteComponent::SpriteParam sprite_param{};
					sprite_param.color = { 1.0f,1.0f, 1.0f, 1.0f };
					sprite_param.filename = "Data/Sprite/DescriptionUI1.png";
					sprite_param.draw_priority = PRIORITY::DEFAULT;
					sprite_param.center_type = Sprite::CENTER_TYPE::CENTER_LEFT;
					const auto& sprite = description_UI1->AddComponent<SpriteComponent>(sprite_param);
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
					BaseSpriteComponent::SpriteParam sprite_param{};
					sprite_param.color = DEFAULT_SPIN_ATTACK_DESCRIPTION_UI_COLOR;
					sprite_param.filename = "Data/Sprite/DescriptionUI2.png";
					sprite_param.draw_priority = PRIORITY::DEFAULT;
					sprite_param.center_type = Sprite::CENTER_TYPE::CENTER_LEFT;
					const auto& sprite = description_UI2->AddComponent<SpriteComponent>(sprite_param);

					sprite->AdjustDisplaySizeToSprite();
				}

				// 更新
				{
					const auto& state_machine = description_UI2->AddComponent<StateMachineComponent>();

					state_machine->RegisterState<SpinAttackDescriptionUIState>("UIState");
					state_machine->SetDefaultState("UIState");
				}
			}
		}
	}

	return descriptionUI;
}

// SetChainScoreCallbacks で使う関数
// weak_ptr が生きていれば shared_ptr にして関数を実行する
template<typename T, typename Func>
void CallIfValid(const std::weak_ptr<T>& wptr, Func&& func)
{
	if (auto sptr = wptr.lock())
	{
		func(*sptr);
	}
}

// UIConstant::CreateChainScoreCounterUIで使用する
// ChainScoreCounterComponent に対して、連鎖スコア演出用のコールバックをまとめて設定する関数
void SetChainScoreCallbacks(
	const std::shared_ptr<ChainScoreCounterComponent>& counter,
	const std::weak_ptr<ScoreUIValueAnimatorComponent>& total,
	const std::weak_ptr<ChainScorePopAnimationComponent>& chain_pop_anim,
	const std::weak_ptr<ChainScoreMoveAnimationComponent>& chain_move_anim)
{
	if (!counter) return;

	// 総スコアのイベント
	counter->AddOnScoreChainEnd(
		[total]()
		{
			CallIfValid(total, [](auto& com) { com.OnScoreChainEnd(); });
		});

	// 連鎖スコアpopのイベント
	counter->AddOnScoreAdded(
		[chain_pop_anim](int value)
		{
			CallIfValid(chain_pop_anim, [value](auto& com) { com.OnScoreAdded(value); });
		});

	counter->AddOnScoreChainStart(
		[chain_pop_anim]()
		{
			CallIfValid(chain_pop_anim, [](auto& com) { com.OnScoreChainStart(); });
		});

	counter->AddOnScoreChainEnd(
		[chain_pop_anim]()
		{
			CallIfValid(chain_pop_anim, [](auto& com) { com.OnScoreChainEnd(); });
		});

	// 連鎖スコアmoveのイベント
	counter->AddOnScoreAdded(
		[chain_move_anim](int value)
		{
			CallIfValid(chain_move_anim, [value](auto& com) { com.OnScoreAdded(value); });
		});
	counter->AddOnScoreChainEnd(
		[chain_move_anim]()
		{
			CallIfValid(chain_move_anim, [](auto& com) { com.OnScoreChainEnd(); });
		});
}

UIConstant::ScoreUIGroup UIConstant::CreateScoreUIs(const std::shared_ptr<Object>& total_ui_object, const std::shared_ptr<Object>& chain_pop_ui_object, const std::shared_ptr<Object>& chain_move_ui_object)
{
	UIConstant::ScoreUIGroup ui_group{};

	ui_group.total_ui_object = total_ui_object;
	ui_group.chain_pop_ui_object = chain_pop_ui_object;
	ui_group.chain_move_ui_object = chain_move_ui_object;

	const DirectX::XMFLOAT2 CHAIN_SCORE_INITIAL_POSITION = { 0.5f, 0.0f };
	const DirectX::XMFLOAT2 CHAIN_SCORE_INITIAL_SCALE = { 1.0f, 1.0f };
	const DirectX::XMFLOAT2 SCORE_TEXT_INITIAL_SCALE = { 2.0f,2.0f };
	const float CHAIN_TIMER_MAX = 2.0f;	// スコア連鎖の猶予時間

	// =========================================
	// 総スコアUIオブジェクト
	// =========================================

	// transform
	{
		Transform2DComponent::Transform2DParam param{};
		param.local_position = { 0.97f,0.015f };
		total_ui_object->AddComponent<Transform2DComponent>(param);
	}

	// 【子オブジェクト】

	// [テキスト]

	const auto& text_sprite = total_ui_object->CreateChildObject("Text");

	// TextNumberComponent
	{
		BaseSpriteComponent::SpriteParam sprite_param{};
		sprite_param.color = { 1.0f,0.0f,0.0f ,1.0f };
		sprite_param.center_type = Sprite::CENTER_TYPE::TOP_RIGHT;
		// ファイルパス設定する
		sprite_param.filename = "Data/Sprite/Numbers.png";
		auto text_number = text_sprite->AddComponent<TextNumberComponent>(sprite_param);
	}

	// transform
	{
		Transform2DComponent::Transform2DParam param{};
		param.local_scale = SCORE_TEXT_INITIAL_SCALE;
		text_sprite->AddComponent<Transform2DComponent>(param);
	}

	// 更新処理
	const float SCORE_TEXT_EXPANDED_RATIO = 0.7f;
	const float SCORE_TEXT_SHRINK_RATIO = 1.0f - SCORE_TEXT_EXPANDED_RATIO;

	ScoreUIValueAnimatorComponent::Param UI_value_animator_param{};
	UI_value_animator_param.animetion_total_time = 0.7f;
	UI_value_animator_param.expanded.ratio = SCORE_TEXT_EXPANDED_RATIO;
	UI_value_animator_param.expanded.target_scale = { 3.0f,3.0f };
	UI_value_animator_param.shrink.ratio = SCORE_TEXT_SHRINK_RATIO;
	UI_value_animator_param.shrink.target_scale = SCORE_TEXT_INITIAL_SCALE;
	const auto& score_UI_value_animator = text_sprite->AddComponent<ScoreUIValueAnimatorComponent>(UI_value_animator_param);
	

	// テキストの値を補間するコンポーネント
	{
		text_sprite->AddComponent<TextNumberValueInterpolatorComponent>();
	}

	// SpriteScalerComponent
	text_sprite->AddComponent<SpriteScalerComponent>();

	// 【子オブジェクト】

	// [背景]
	const auto bg_sprite = text_sprite->CreateChildObject("BG_Sprite");

	// 背景画像
	{
		BaseSpriteComponent::SpriteParam sprite_param{};
		sprite_param.display_size = { 0.215f,0.015f };
		sprite_param.color = { 1.0f,1.0f, 1.0f, 1.0f };
		sprite_param.draw_priority = PRIORITY::LOW;
		sprite_param.center_type = Sprite::CENTER_TYPE::TOP_RIGHT;
		const auto& sprite = bg_sprite->AddComponent<SpriteComponent>(sprite_param);
	}

	// transform
	{
		Transform2DComponent::Transform2DParam param{};
		param.local_position = { 0.0f,0.085f };
		param.local_scale = { 0.5f,0.5f };
		bg_sprite->AddComponent<Transform2DComponent>(param);
	}


	// =========================================
	// 連鎖スコア移動UIオブジェクト
	// =========================================

	// transform
	{
		Transform2DComponent::Transform2DParam param{};
		param.local_position = CHAIN_SCORE_INITIAL_POSITION;
		param.local_scale = CHAIN_SCORE_INITIAL_SCALE;
		chain_pop_ui_object->AddComponent<Transform2DComponent>(param);
	}

	// SpriteScalerComponent
	chain_pop_ui_object->AddComponent<SpriteScalerComponent>();

	// 連鎖スコアUIの出現演出を管理

	const float CHAIN_SCORE_POP_EXPANDED_RATIO = 0.2f;
	const float CHAIN_SCORE_POP_SHRINK_RATIO = 1.0f - CHAIN_SCORE_POP_EXPANDED_RATIO;

	ChainScorePopAnimationComponent::Param chain_score_pop_animation_param{};
	chain_score_pop_animation_param.fead_in_time = 0.3f;
	chain_score_pop_animation_param.scale_transition_total_time = CHAIN_TIMER_MAX;
	chain_score_pop_animation_param.expanded.target_scale = { 2.5f,2.5f };
	chain_score_pop_animation_param.expanded.ratio = CHAIN_SCORE_POP_EXPANDED_RATIO;
	chain_score_pop_animation_param.shrink.target_scale = CHAIN_SCORE_INITIAL_SCALE;
	chain_score_pop_animation_param.shrink.ratio = CHAIN_SCORE_POP_SHRINK_RATIO;
	const auto& chain_score_pop_animation = chain_pop_ui_object->AddComponent<ChainScorePopAnimationComponent>(chain_score_pop_animation_param);

	// チェインスコアを管理するコンポーネント
	ChainScoreCounterComponent::Param param{};
	param.chain_timer_max = CHAIN_TIMER_MAX;
	const auto& chain_score_counter = chain_pop_ui_object->AddComponent<ChainScoreCounterComponent>(param);


	// フェード管理コンポーネント
	{
		FadeControllerComponent::FadeControllerParam param{};
		chain_pop_ui_object->AddComponent<FadeControllerComponent>(param);
	}

	// 数値表示オブジェクト
	{
		BaseSpriteComponent::SpriteParam sprite_param{};
		sprite_param.color = { 1.0,1.0f,1.0f ,0.0f };
		sprite_param.center_type = Sprite::CENTER_TYPE::TOP_CENTER;
		// ファイルパス設定する
		sprite_param.filename = "Data/Sprite/Numbers.png";
		const auto& text_number = chain_pop_ui_object->AddComponent<TextNumberComponent>(sprite_param);
	}

	// =========================================
	// 連鎖スコア移動UIオブジェクト
	// =========================================

	// transform
	{
		Transform2DComponent::Transform2DParam param{};
		param.local_position = CHAIN_SCORE_INITIAL_POSITION;
		param.local_scale = CHAIN_SCORE_INITIAL_SCALE;
		chain_move_ui_object->AddComponent<Transform2DComponent>(param);
	}

	// SpriteMoverComponent
	chain_move_ui_object->AddComponent<SpriteMoverComponent>();

	// 連鎖スコアUIを合計スコアへ移動させるコンポーネント
	ChainScoreMoveAnimationComponent::Param chain_score_move_animation_param{};
	chain_score_move_animation_param.target_pos = { 0.95f, 0.0f };
	chain_score_move_animation_param.initial_pos = CHAIN_SCORE_INITIAL_POSITION;
	const auto& chain_score_move_animation = chain_move_ui_object->AddComponent<ChainScoreMoveAnimationComponent>(chain_score_move_animation_param);

	// フェード管理コンポーネント
	{
		FadeControllerComponent::FadeControllerParam param{};
		chain_move_ui_object->AddComponent<FadeControllerComponent>(param);
	}

	// 数値表示オブジェクト
	{
		BaseSpriteComponent::SpriteParam sprite_param{};
		sprite_param.color = { 1.0,1.0f,1.0f ,0.0f };
		sprite_param.center_type = Sprite::CENTER_TYPE::TOP_CENTER;
		// ファイルパス設定する
		sprite_param.filename = "Data/Sprite/Numbers.png";
		const auto& text_number = chain_move_ui_object->AddComponent<TextNumberComponent>(sprite_param);
	}


	// =========================================
	//  ChainScoreCounterComponentのコールバック変数を設定する
	// =========================================
	SetChainScoreCallbacks(chain_score_counter, score_UI_value_animator, chain_score_pop_animation, chain_score_move_animation);

	return ui_group;
}
