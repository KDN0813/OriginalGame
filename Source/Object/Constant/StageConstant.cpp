#include "StageConstant.h"

#include "Object\Object.h"
#include "Shader\ModelShader.h"

#include "Component\ModelComponent.h"
#include "Component\TransformComponent.h"
#include "Component\ModelShaderComponent.h"

const std::shared_ptr<Object>& StageConstant::CreateStageFloor(const std::shared_ptr<Object>& stage)
{
	auto model = stage->AddComponent<ModelComponent>("Data/Model/Stage/Floor/StageFloor.mdl");
	model->SetTileCount(20.0f);
	// トランスフォーム設定
	{
		Transform3DComponent::Transform3DParam param{};
		const float scale = 600.0f;
		param.local_scale = DirectX::XMFLOAT3(scale, 1.0f, scale);
		param.local_position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		auto transform = stage->AddComponent<Transform3DComponent>(param);
	}
	// シェーダー設定
	{
		ModelShader::Instance model_shader = ModelShader::GetInstance();
		const auto& shader_component =
			stage->AddComponent<ModelShaderComponent>(model_shader.Get());
	}

	return stage;
}

const std::shared_ptr<Object>& StageConstant::CreateStageWall(const std::shared_ptr<Object>& object)
{
	auto model = object->AddComponent<ModelComponent>("Data/Model/Stage/Wall/StageWall.mdl");
	// トランスフォーム設定
	{
		Transform3DComponent::Transform3DParam param{};
		const float scale = 1.0f;
		param.local_scale = DirectX::XMFLOAT3(scale, scale, scale);
		param.local_position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		auto transform = object->AddComponent<Transform3DComponent>(param);
	}
	// シェーダー設定
	{
		ModelShader::Instance model_shader = ModelShader::GetInstance();
		const auto& shader_component =
			object->AddComponent<ModelShaderComponent>(model_shader.Get());
	}

	return object;
}
