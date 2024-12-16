#include "UIConstant.h"
#include "Object\Object.h"
#include "Shader\SpriteShader.h"

#include "Component\TextNumberComponent.h"
#include "Component\SpriteComponent.h"
#include "Component\StateMachineComponent.h"
#include "Component\Transform2DComponent.h"

#include "StateMachine\UIStateDerived.h"

const std::shared_ptr<Object>& UIConstant::CreateScoreUI(const std::shared_ptr<Object>& score_object)
{
	// �e�L�X�g�\��
	{
		TextNumberComponent::TextParam param{};
		param.color = { 1.0f,0.0f,0.0f ,1.0f };
		param.center_type = Sprite::CENTER_TYPE::CENTER;
		// �t�@�C���p�X�ݒ肷��
		param.font_name = "Data/Sprite/Numbers.png";
		auto text_number = score_object->AddComponent<TextNumberComponent>(param);

		if (SpriteShader::Instance sprite_shader = SpriteShader::GetInstance(); sprite_shader.Get())
		{
			sprite_shader->AddSprite(text_number);
		}
	}

	// transform
	{
		Transform2DComponent::Transform2DParam paam{};
		paam.local_position = { 0.45f,-0.03f };
		score_object->AddComponent<Transform2DComponent>(paam);
	}

	// �X�V����
	{
		auto state_machine = score_object->AddComponent<StateMachineComponent>();

		state_machine->RegisterState<ScoreUIDefaultState>();

		state_machine->SetDefaultState(ScoreUIDefaultState::STATE_NAME);
	}

	// �q�I�u�W�F�N�g
	{
		// �w�i
		{
			const auto bg_sprite = score_object->CreateChildObject("BG_Sprite");

			// transform
			{
				Transform2DComponent::Transform2DParam paam{};
				paam.local_position = { 0.45f,-0.03f };
				score_object->AddComponent<Transform2DComponent>(paam);
			}
		}
	}

	return score_object;
}

const std::shared_ptr<Object>& UIConstant::CreateGameTimerUI(const std::shared_ptr<Object>& sprite_shader)
{
	// �e�L�X�g�\��
	{
		TextNumberComponent::TextParam param{};
		param.color = { 1.0f,0.0f,0.0f ,1.0f };
		param.center_type = Sprite::CENTER_TYPE::CENTER;
		// �t�@�C���p�X�ݒ肷��
		param.font_name = "Data/Sprite/Numbers.png";
		auto text_number = sprite_shader->AddComponent<TextNumberComponent>(param);

		if (SpriteShader::Instance sprite_shader = SpriteShader::GetInstance(); sprite_shader.Get())
		{
			sprite_shader->AddSprite(text_number);
		}
	}

	// transform
	{
		Transform2DComponent::Transform2DParam paam{};
		paam.local_position = { 0.45f,0.1f };
		sprite_shader->AddComponent<Transform2DComponent>(paam);
	}

	// �X�V����
	{
		auto state_machine = sprite_shader->AddComponent<StateMachineComponent>();

		state_machine->RegisterState<EndTimerUIDefaultState>();

		state_machine->SetDefaultState(EndTimerUIDefaultState::STATE_NAME);
	}

	return sprite_shader;
}

const std::shared_ptr<Object>& UIConstant::CreatePause(const std::shared_ptr<Object>& pause_object)
{
	// �X�v���C�g�ǂݍ���
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
