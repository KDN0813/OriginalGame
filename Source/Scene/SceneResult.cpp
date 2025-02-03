#include "SceneResult.h"
#include "Graphics/Graphics.h"

// �V�F�[�_�[
#include "Shader/SpriteShader.h"

#include "Scene/SceneManager.h"
#include "Scene/SceneTitle.h"

#include "Component/SpriteComponent.h"
#include "Component/Transform2DComponent.h"
#include "Component/TextNumberComponent.h"
#include "Component/StateMachineComponent.h"

#include "StateMachine\UIStateDerived.h"


void SceneResult::Initialize()
{
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
						SpriteComponent::SpriteParam param{};
						param.filename = "Data/Sprite/TitleBack.png";
						auto sprite = BG->AddComponent<SpriteComponent>(param);
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
						SpriteComponent::SpriteParam param{};
						param.filename = "Data/Sprite/ResultBack.png";
						auto sprite = frame->AddComponent<SpriteComponent>(param);
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
						SpriteComponent::SpriteParam param{};
						param.filename = "Data/Sprite/ResultText.png";
						param.center_type = Sprite::CENTER_TYPE::CENTER;
						auto sprite = score_text->AddComponent<SpriteComponent>(param);
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
						TextNumberComponent::TextParam param{};
						param.color = { 0.0f,0.0f,0.0f ,1.0f };
						param.center_type = Sprite::CENTER_TYPE::CENTER_RIGHT;
						// �t�@�C���p�X�ݒ肷��
						param.font_name = "Data/Sprite/Numbers.png";
						auto text_number = score->AddComponent<TextNumberComponent>(param);
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

						state_machine->RegisterState<ScoreUIDefaultState>();

						state_machine->SetDefaultState(ScoreUIDefaultState::STATE_NAME);
					}
				}
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

void SceneResult::ReStart()
{
	this->object_manager.ReStart();
}

void SceneResult::DebugDrawGUI()
{
	this->object_manager.DrawDebugGUI();
}
