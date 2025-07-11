#include "SceneTitle.h"
#include "Scene/SceneLoading.h"
#include "Scene/SceneGame.h"
#include "Scene/SceneManager.h"
#include "Graphics/Graphics.h"
#include "Input\Input.h"
#include "System\GameData.h"
#include "Audio\Audio.h"
#ifdef _DEBUG
#include "Debug/ImGuiHelper.h"
#endif // DEBUG

#include "Shader/SpriteShader.h"

#include "Component/SpriteComponent.h"
#include "Component\Transform2DComponent.h"

void SceneTitle::Initialize()
{
	// �Q�[����Ԃ�ݒ�
	{
		GameData::Instance game_data = GameData::GetInstance();
		game_data->SetGameStatus(GameData::GameStatus::TITLE);
	}

	// �I�u�W�F�N�g�쐬
	{
		// �^�C�g���w�i
		{
			auto sprite_bg = object_manager.Create("Title Back");

			// �X�v���C�g�ǂݍ���
			{
				BaseSpriteComponent::SpriteParam sprite_param{};
				sprite_param.color = { 1.0f,1.0f, 1.0f, 1.0f };
				sprite_param.filename = "Data/Sprite/TitleBack.png";
				auto sprite = sprite_bg->AddComponent<SpriteComponent>(sprite_param);
			}

			// transform
			{
				Transform2DComponent::Transform2DParam paam{};
				sprite_bg->AddComponent<Transform2DComponent>(paam);
			}
		}

		// �^�C�g�����S
		{
			auto sprite_logo = object_manager.Create();
			sprite_logo->SetName("Title Logo");

			// �X�v���C�g�ǂݍ���
			{
				BaseSpriteComponent::SpriteParam sprite_param{};
				sprite_param.display_size = { 0.55f,0.35f };
				sprite_param.color = { 1.0f,1.0f, 1.0f, 1.0f };
				sprite_param.filename = "Data/Sprite/TitleLogo.png";
				sprite_param.center_type = Sprite::CENTER_TYPE::CENTER;
				auto sprite = sprite_logo->AddComponent<SpriteComponent>(sprite_param);
				sprite->AdjustDisplaySizeToSprite();
			}

			// transform
			{
				Transform2DComponent::Transform2DParam param{};
				param.local_position = { 0.5f,0.2f };
				param.local_scale = { 1.5f,1.5f };
				sprite_logo->AddComponent<Transform2DComponent>(param);
			}
		}

		// �Q�[���X�^�[�g�e�L�X�g
		{
			auto sprite_bg = object_manager.Create("Game Staet Text");
			{
				BaseSpriteComponent::SpriteParam sprite_param{};
				sprite_param.color = { 1.0f,1.0f, 1.0f, 1.0f };
				sprite_param.center_type = Sprite::CENTER_TYPE::CENTER;
				sprite_param.filename = "Data/Sprite/TitleGameStartText.png";
				auto sprite = sprite_bg->AddComponent<SpriteComponent>(sprite_param);
				sprite->AdjustDisplaySizeToSprite();

			}

			// transform
			{
				Transform2DComponent::Transform2DParam param{};
				param.local_position = { 0.5f,0.630f };
				param.local_scale = { 0.4f,0.4f };
				sprite_bg->AddComponent<Transform2DComponent>(param);
			}
		}

		// �Q�[���I���e�L�X�g
		{
			auto sprite_bg = object_manager.Create("Game Staet End");
			{
				BaseSpriteComponent::SpriteParam sprite_param{};
				sprite_param.color = { 1.0f,1.0f, 1.0f, 1.0f };
				sprite_param.center_type = Sprite::CENTER_TYPE::CENTER;
				sprite_param.filename = "Data/Sprite/TitleGameEndText.png";
				auto sprite = sprite_bg->AddComponent<SpriteComponent>(sprite_param);
				sprite->AdjustDisplaySizeToSprite();
			}

			// transform
			{
				Transform2DComponent::Transform2DParam param{};
				param.local_position = { 0.5f,0.87f };
				param.local_scale = { 0.4f,0.4f };
				sprite_bg->AddComponent<Transform2DComponent>(param);
			}
		}
	}

	// �I�[�f�B�I�Đ�
	{
		Audio::Instance audio = Audio::GetInstance();
		AudioParam param{};
		param.volume = 0.5f;
		param.loop = true;
		param.filename = "Data/Audio/BGM.wav";
		audio->Play(param);
	}
}

void SceneTitle::Finalize()
{
	// �I�[�f�B�I�̃��Z�b�g
	{
		Audio::Instance audio = Audio::GetInstance();
		audio->AllClear();
	}
}

void SceneTitle::Update(float elapsed_time)
{
	object_manager.Update(elapsed_time);

	// �V�[���J��
	{
		Input::Instance input = Input::GetInstance();
		const auto& game_pad = input->GetGamePad();

		// X�{�^���������ꂽ��
		if (GamePad::BTN_X & game_pad.GetButtonDown())
		{
			SceneManager::Instance scene_manager = SceneManager::GetInstance();
			scene_manager->ChangeScene(new SceneLoading(new SceneGame()));
		}
		// [Y�{�^��/ESC�L�[] �������ꂽ��
		else if((GamePad::BTN_Y | GamePad::BTN_EXIT) & game_pad.GetButtonDown())
		{
			GameData::Instance game_data = GameData::GetInstance();
			// �E�B���h�E���I��������
			game_data->CloseWindow();
		}
	}
}

void SceneTitle::Render()
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

void SceneTitle::ReStart()
{
	this->object_manager.ReStart();
}

#ifdef _DEBUG

void SceneTitle::DebugDrawGUI()
{
	// Object
	this->object_manager.DrawDebugGUI();

	// �V�F�[�_�[
	{
		if (ImGui::Begin("Sahder"))
		{
			SpriteShader::Instance sprite_shader = SpriteShader::GetInstance();
			sprite_shader->DrawDebugGUI();
		}
		ImGui::End();
	}
}

#endif // DEBUG