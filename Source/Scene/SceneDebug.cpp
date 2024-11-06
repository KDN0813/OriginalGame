#include "SceneDebug.h"
#include "Graphics/Graphics.h"
#include "imgui.h"
#include "System/Misc.h"
#include "Debug/DebugCereal.h"
#include "Camera/CameraManager.h"

#include "Scene/SceneManager.h"

#include "Component/ModelComponent.h"
#include "Component/ModelShaderComponent.h"
#include "Component/InstancedModelWithStateAnimationComponent.h"
#include "Component/TransformComponent.h"
#include "Component/InstancingModelShaderComponent.h"
#include "Component/MovementComponent.h"
#include "Component/CameraComponent.h"

SceneDebug::SceneDebug()
{
}

void SceneDebug::Initialize()
{
	Graphics::Instance graphics = Graphics::GetInstance();
	if (!graphics.Get()) return;
	ID3D11Device* device = graphics->GetDevice();
	//// シェーダーの作成
	//{
	//	instancing_model_shader = std::make_unique<InstancingModelShader>(device);
	//	model_shader = std::make_unique<ModelShader>(device);
	//}

	//// カメラ作成
	//{
	//}

	//// デバッグオブジェクト作成
	//{
	//	// ステージ
	//	{
	//		auto stage = object_manager.Create();
	//		stage->SetName("Stage");
	//		stage->AddComponent<ModelComponent>(device,"Data/Model/ExampleStage/ExampleStage.mdl");
	//		auto transform = stage->AddComponent<Transform3DComponent>();
	//		transform->SetLocalScale(DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));
	//		// シェーダー設定
	//		auto shader_component =
	//			stage->AddComponent<ModelShaderComponent>(model_shader.get());
	//		model_shader->AddShaderComponent(shader_component.get());
	//	}

	//	// ボックス
	//	{
	//		auto cube = object_manager.Create();
	//		cube->SetName("Cube");
	//		cube->AddComponent<ModelComponent>(device,"Data/Model/Cube/Cube.mdl");
	//		auto transform = cube->AddComponent<Transform3DComponent>();
	//		transform->SetLocalScale(DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));
	//		transform->SetLocalPosition(DirectX::XMFLOAT3(5.0f, 2.0f, 0.0f));
	//		transform->SetLocalAngle(DirectX::XMFLOAT3(DirectX::XMConvertToRadians(90.0f), 0.0f, 0.0f));
	//		// シェーダー設定
	//		auto shader_component =
	//			cube->AddComponent<ModelShaderComponent>(model_shader.get());
	//		model_shader->AddShaderComponent(shader_component.get());
	//	}

	//	InstancingModelShader* const shader = instancing_model_shader.get();
	//	// インスタンシング描画テスト
	//	{
	//		float offset = 3.0f;
	//		for (int x = 0; x < 10; ++x)
	//		{
	//			for (int z = 0; z < 10; ++z)
	//			{
	//				DirectX::XMFLOAT3 pos =
	//				{
	//					static_cast<float>(x) * offset,
	//					0.0f,
	//					static_cast<float>(z) * offset,
	//				};
	//				auto object = object_manager.Create();
	//				auto transform = object->AddComponent<Transform3DComponent>();
	//				auto model = object->AddComponent<AnimatedInstancedModelComponent>(device, "Data/Model/Jammo/Jammo.mdl");
	//				// シェーダー設定
	//				auto shader_component =
	//					object->AddComponent<InstancingModelShaderComponent>(shader);
	//				shader->AddShaderComponent(shader_component.get());

	//				model->PlayAnimation(z);
	//				transform->SetLocalPosition(pos);
	//				transform->SetLocalScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
	//			}
	//		}
	//	}
	//}
}

void SceneDebug::Finalize()
{
}

void SceneDebug::Update(float elapsed_time)
{
	object_manager.Update(elapsed_time);

	if (CameraManager::Instance camera_manager = CameraManager::GetInstance(); camera_manager.Get())
	{
		camera_manager->Update(elapsed_time);
	}
}

void SceneDebug::Render()
{
	// 画面のクリア
	{
		Graphics::Instance graphics = Graphics::GetInstance();
		if (!graphics.Get()) return;
		graphics->PrepareRenderTargets();
	}

	// 3Dモデルの描画
	{
		// モデル描画
		this->model_shader->Render();

		// インスタンシング描画
		this->instancing_model_shader->Render();
	}

#ifdef _DEBUG
	DebugDrawGUI();
#endif // _DEBUG
}

#ifdef _DEBUG

void SceneDebug::DebugDrawGUI()
{
}

void SceneDebug::DrawShaderImGui()
{
	if (ImGui::CollapsingHeader(model_shader->GetName(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		model_shader->DrawDebugGUI();
	}
	if (ImGui::CollapsingHeader(instancing_model_shader->GetName(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		instancing_model_shader->DrawDebugGUI();
	}
}

#endif // _DEBUG