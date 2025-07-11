#include "SceneResult.h"
#include "Graphics/Graphics.h"
#include "Input\Input.h"
#include "System\GameData.h"

// �V�F�[�_�[
#include "Shader/SpriteShader.h"

#include "Scene/SceneManager.h"
#include "Scene/SceneTitle.h"
#include "Scene/SceneGame.h"
#include "Scene/SceneLoading.h"

#include "Component/SpriteComponent.h"
#include "Component/Transform2DComponent.h"
#include "Component/TextNumberComponent.h"
#include "Component/StateMachineComponent.h"
#include "Component/FadeControllerComponent.h"

#include "StateMachine\UIStateDerived.h"

void SceneResult::Initialize()
{
	// �t�F�[�h����
	{
		const auto& fade_controlle = SceneManager::GetInstance()->GetFadeControlle();
		fade_controlle->ClearFade();
	}

	// �I�u�W�F�N�g�쐬
	{
		// �w�i
		{
			auto sprite_bg = object_manager.Create("Result Back");

			// transform
			{
				Transform2DComponent::Transform2DParam param{};
				sprite_bg->AddComponent<Transform2DComponent>(param);
			}

			// �q�I�u�W�F�N�g
			{
				// �w�i
				{
					const auto& BG = sprite_bg->CreateChildObject("BG");
					// �X�v���C�g
					{
						BaseSpriteComponent::SpriteParam sprite_param{};
						sprite_param.filename = "Data/Sprite/TitleBack.png";
						auto sprite = BG->AddComponent<SpriteComponent>(sprite_param);
					}
					// transform
					{
						Transform2DComponent::Transform2DParam param{};
						BG->AddComponent<Transform2DComponent>(param);
					}
				}
				// �t���[��
				{
					const auto& frame = sprite_bg->CreateChildObject("Frame");
					// �X�v���C�g
					{
						BaseSpriteComponent::SpriteParam sprite_param{};
						sprite_param.filename = "Data/Sprite/ResultBack.png";
						auto sprite = frame->AddComponent<SpriteComponent>(sprite_param);
					}
					// transform
					{
						Transform2DComponent::Transform2DParam param{};
						frame->AddComponent<Transform2DComponent>(param);
					}
				}
			}
		}

		// �X�R�AUI
		{
			auto scoreUI = object_manager.Create("Result Text");

			// transform
			{
				Transform2DComponent::Transform2DParam param{};
				scoreUI->AddComponent<Transform2DComponent>(param);
			}

			// �q�I�u�W�F�N�g
			{
				// �e�L�X�g
				{
					const auto& score_text = scoreUI->CreateChildObject("Text");
					// �X�v���C�g
					{
						BaseSpriteComponent::SpriteParam sprite_param{};
						sprite_param.filename = "Data/Sprite/ResultText.png";
						sprite_param.center_type = Sprite::CENTER_TYPE::CENTER;
						auto sprite = score_text->AddComponent<SpriteComponent>(sprite_param);
						sprite->AdjustDisplaySizeToSprite();
					}
					// transform
					{
						Transform2DComponent::Transform2DParam param{};
						param.local_scale = { 0.5f,0.5f };
						param.local_position = { 0.5f,0.2f };
						score_text->AddComponent<Transform2DComponent>(param);
					}
				}

				// �X�R�A
				{
					const auto& score = scoreUI->CreateChildObject("Score");

					// TextNumberComponent
					{
						BaseSpriteComponent::SpriteParam sprite_param{};
						sprite_param.color = { 1.0f,0.0f,0.0f ,1.0f };
						sprite_param.center_type = Sprite::CENTER_TYPE::CENTER_RIGHT;
						// �t�@�C���p�X�ݒ肷��
						sprite_param.filename = "Data/Sprite/Numbers.png";
						auto text_number = score->AddComponent<TextNumberComponent>(sprite_param);
					}

					// transform
					{
						Transform2DComponent::Transform2DParam param{};
						param.local_position = { 0.84f,0.46f };
						param.local_scale = { 3.0f,3.0f };
						score->AddComponent<Transform2DComponent>(param);
					}

					// �X�V����
					{
						auto state_machine = score->AddComponent<StateMachineComponent>();

						state_machine->RegisterState<ScoreUIDefaultState>("UIState");

						state_machine->SetDefaultState("UIState");
					}
				}
			}
		}

		// ���U���g�e�L�X�g
		{
			auto score_text = object_manager.Create("Score Text");
			// �X�v���C�g
			{
				BaseSpriteComponent::SpriteParam sprite_param{};
				sprite_param.filename = "Data/Sprite/ScoreText.png";
				sprite_param.center_type = Sprite::CENTER_TYPE::CENTER;
				auto sprite = score_text->AddComponent<SpriteComponent>(sprite_param);
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
				BaseSpriteComponent::SpriteParam sprite_param{};
				sprite_param.filename = "Data/Sprite/ResultUI.png";
				sprite_param.center_type = Sprite::CENTER_TYPE::CENTER;
				auto sprite = resultUI->AddComponent<SpriteComponent>(sprite_param);
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
	{
		Input::Instance input = Input::GetInstance();
		GamePad game_pad = input->GetGamePad();

		// (X)���g���C
		if (GamePad::BTN_X & game_pad.GetButtonDown())
		{
			GameData::Instance game_data = GameData::GetInstance();
			game_data->SetGameStatus(GameData::GameStatus::GAME);
			SceneManager::Instance scene_manager = SceneManager::GetInstance();
			scene_manager->ChangeScene(new SceneLoading(new SceneGame));
			return;
		}
		// (Y)�^�C�g����
		else if (GamePad::BTN_Y & game_pad.GetButtonDown())
		{
			GameData::Instance game_data = GameData::GetInstance();
			game_data->SetGameStatus(GameData::GameStatus::TITLE);
			SceneManager::Instance scene_manager = SceneManager::GetInstance();
			scene_manager->ChangeScene(new SceneTitle);
			return;
		}
	}

	this->object_manager.Update(elapsed_time);
}

void SceneResult::Render()
{
	// �`�揀��
	Graphics::Instance graphics = Graphics::GetInstance();
	graphics->PrepareRenderTargets();

	// 2D�X�v���C�g�`��
	{
		SpriteShader::Instance sprite_shader = SpriteShader::GetInstance();
		sprite_shader->Render();
	}
}

void SceneResult::ReStart()
{
	this->object_manager.ReStart();
}

#ifdef _DEBUG

void SceneResult::DebugDrawGUI()
{
	this->object_manager.DrawDebugGUI();
}

#endif // _DEBUG