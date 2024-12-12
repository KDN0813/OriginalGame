#include "UIConstant.h"
#include "Object\Object.h"
#include "Shader\SpriteShader.h"

#include "Component\TextNumberComponent.h"
#include "Component\StateMachineComponent.h"

#include "StateMachine\UIStateDerived.h"

const std::shared_ptr<Object>& UIConstant::CreateScoreUI(const std::shared_ptr<Object>& score_object)
{
	// テキスト表示
	{
		TextNumberComponent::TextParam param{};
		param.pos = { 0.45f,-0.03f };
		param.color = { 1.0f,0.0f,0.0f ,1.0f };
		param.center_type = Sprite::CENTER_TYPE::CENTER;
		// ファイルパス設定する
		param.font_name = "Data/Sprite/Numbers.png";
		auto text_number = score_object->AddComponent<TextNumberComponent>(param);

		if (SpriteShader::Instance sprite_shader = SpriteShader::GetInstance(); sprite_shader.Get())
		{
			sprite_shader->AddSprite(text_number);
		}
	}

	// 更新処理
	{
		auto state_machine = score_object->AddComponent<StateMachineComponent>();

		state_machine->RegisterState<ScoreUIDefaultState>();

		state_machine->SetDefaultState("ScoreUIDefaultState");
	}

	return score_object;
}
