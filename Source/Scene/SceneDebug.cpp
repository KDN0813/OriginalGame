#include "SceneDebug.h"
#include "Graphics/Graphics.h"
#include "imgui.h"
#include "System/Misc.h"
#include "Debug/DebugCereal.h"
#include "Camera/CameraManager.h"

#include "Scene/SceneManager.h"

#include "Component/ModelComponent.h"
#include "Component/ModelShaderComponent.h"
#include "Component/InstancingModelComponent.h"
#include "Component/TransformComponent.h"
#include "Component/InstancingModelShaderComponent.h"
#include "Component/MovementComponent.h"
#include "Component/CameraComponent.h"

SceneDebug::SceneDebug()
{
}

void SceneDebug::Initialize()
{
	Graphics* graphics = Graphics::Instance();
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

	CameraManager::Instance()->Update(elapsed_time);
}

void SceneDebug::Render()
{
	Graphics* graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics->GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics->GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics->GetDepthStencilView();

	FLOAT color[] = { 0.5f, 1.0f, 1.0f, 1.0f };
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 3Dモデルの描画
	{
		Graphics* graphics = Graphics::Instance();
		ID3D11DeviceContext* dc = graphics->GetDeviceContext();
		//Camera& camera = Camera::Intance();

		CameraManager* camera_manager = CameraManager::Instance();
		auto camera= camera_manager->GetMainCamera();
		RenderContext rc;
		rc.view = camera->GetViewTransform();
		rc.projection = camera->GetProjectionTransform();

		// モデル描画
		this->model_shader->Render(dc, rc);

		// インスタンシング描画
		this->instancing_model_shader->Render(dc, rc);
	}

#ifdef _DEBUG
	DrawImGui();
#endif // _DEBUG
}

#ifdef _DEBUG

void SceneDebug::DrawImGui()
{
	ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(ImVec2(300.0f, 400.0f), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("DebugMenu", nullptr, ImGuiWindowFlags_MenuBar))
	{
		// モード切り替え処理
		if (ImGui::BeginMenuBar()) 
		{
			if (ImGui::BeginMenu("Mode"))
			{
				if (ImGui::MenuItem("Object")) 
				{
					mode_index = SceneDebug::ImGuiMode::Object;
				}
				if (ImGui::MenuItem("Shader")) 
				{
					mode_index = SceneDebug::ImGuiMode::Shader;
				}
				if (ImGui::MenuItem("Camera")) 
				{
					mode_index = SceneDebug::ImGuiMode::Camera;
				}
				if (ImGui::MenuItem("System")) 
				{
					mode_index = SceneDebug::ImGuiMode::System;
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// 各モード表示
		switch (mode_index)
		{
		case SceneDebug::ImGuiMode::Object:
			ImGui::Text("Object");
			object_manager.DrawDebugGUI();
			break;
		case SceneDebug::ImGuiMode::Shader:
			ImGui::Text("System");
			DrawShaderImGui();
			break;
		case SceneDebug::ImGuiMode::Camera:
			ImGui::Text("Camera");
			CameraManager::Instance()->DrawDebugGUI();
			break;
		case SceneDebug::ImGuiMode::System:
			ImGui::Text("System");
			break;
		default:
			break;
		}
	}
	ImGui::End();
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