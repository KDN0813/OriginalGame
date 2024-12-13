#include "StageConstant.h"

#include "Object\Object.h"
#include "Shader\ModelShader.h"

#include "Component\ModelComponent.h"
#include "Component\TransformComponent.h"
#include "Component\ModelShaderComponent.h"

const std::shared_ptr<Object>& StageConstant::CreateStage(const std::shared_ptr<Object>& stage)
{
	auto model = stage->AddComponent<ModelComponent>("Data/Debug/Model/Cube/Cube2.mdl");
	model->SetTileCount(150.0f);
	// トランスフォーム設定
	{
		Transform3DComponent::Transform3DParam param{};
		const float scale = 600.0f;
		param.local_scale = DirectX::XMFLOAT3(scale, 1.0f, scale);
		param.local_position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		auto transform = stage->AddComponent<Transform3DComponent>(param);
	}
	// シェーダー設定
	if(ModelShader::Instance model_shader = ModelShader::GetInstance(); model_shader.Get())
	{
		auto shader_component =
			stage->AddComponent<ModelShaderComponent>(model_shader.Get());
	}

	return stage;
}
