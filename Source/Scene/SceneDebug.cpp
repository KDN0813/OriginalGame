#include "SceneDebug.h"
#include "Graphics/Graphics.h"
#include "imgui.h"
#include "System/Misc.h"
#include "Debug/DebugCereal.h"

#include "Camera/Camera.h"

#include "Component/ModelComponent.h"
#include "Component/ModelShaderComponent.h"
#include "Component/InstancingModelComponent.h"
#include "Component/TransformComponent.h"
#include "Component/InstancingModelShaderComponent.h"
#include "Component/MovementComponent.h"

SceneDebug::SceneDebug()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11Device* device = graphics.GetDevice();
	// シェーダーの作成
	{
		instance_model_shader = std::make_unique<InstancingModelShader>(device);
		model_shader = std::make_unique<ModelShader>(device);
	}
	
	// デバッグオブジェクト作成
	{
		// ステージ
		{
			auto stage = object_manager.Create();
			stage->SetName("Stage");
			stage->AddComponent<ModelComponent>("Data/Model/ExampleStage/ExampleStage.mdl");
			auto transform = stage->AddComponent<Transform3DComponent>();
			transform->SetScale(DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));
			// シェーダー設定
			auto shader_component =
				stage->AddComponent<ModelShaderComponent>(model_shader.get());
			model_shader->AddShaderComponent(shader_component);
		}

		// ボックス
		{
			auto cube = object_manager.Create();
			cube->SetName("Cube");
			cube->AddComponent<ModelComponent>("Data/Model/Cube/Cube.mdl");
			auto transform = cube->AddComponent<Transform3DComponent>();
			transform->SetScale(DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));
			transform->SetPosition(DirectX::XMFLOAT3(5.0f, 10.0f, 0.0f));
			transform->SetQuaternion(DirectX::XMFLOAT4(DirectX::XMConvertToRadians(90), 0.0f, 0.0f, 1.0f));
			// シェーダー設定
			auto shader_component =
				cube->AddComponent<ModelShaderComponent>(model_shader.get());
			model_shader->AddShaderComponent(shader_component);
		}

		InstancingModelShader* const shader = instance_model_shader.get();
		// インスタンシング描画テスト
		{
			float offset = 3.0f;
			for (int x = 0; x < 10; ++x)
			{
				for (int z = 0; z < 10; ++z)
				{
					DirectX::XMFLOAT3 pos =
					{
						static_cast<float>(x) * offset,
						0.0f,
						static_cast<float>(z) * offset,
					};
					auto object = object_manager.Create();
					auto transform = object->AddComponent<Transform3DComponent>();
					auto model = object->AddComponent<InstancingModelComponent>(device, "Data/Model/Jammo/Jammo.mdl");
					// シェーダー設定
					auto shader_component =
						object->AddComponent<InstancingModelShaderComponent>(shader);
					shader->AddShaderComponent(shader_component);

					model->PlayAnimetion(z);
					transform->SetPosition(pos);
					transform->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
				}
			}
		}
	}
}

void SceneDebug::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	// カメラ初期設定
	Camera& camera = Camera::Intance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0.0f, 0.0f, -10.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 10.0f, 0.0f)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45.0f),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);
}

void SceneDebug::Finalize()
{
}

void SceneDebug::Update(float elapsed_time)
{
	// カメラコントローラー更新処理
	DirectX::XMFLOAT3 target = {};
	target.y += 0.5f;	// プレイヤーの腰当たりをターゲットに設定
	cameraController.SetTarget(target);
	cameraController.Update(elapsed_time);

	object_manager.Update(elapsed_time);
}

void SceneDebug::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	FLOAT color[] = { 0.5f, 1.0f, 0.5f, 1.0f };
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 3Dモデルの描画
	{
		Graphics& graphics = Graphics::Instance();
		ID3D11DeviceContext* dc = graphics.GetDeviceContext();
		FormerInstanceShader* instance_shader = graphics.GetInstanceShader();
		DebugShader* temporary_shader = graphics.GetTemporaryShader();
		Camera& camera = Camera::Intance();
		RenderContext rc;
		rc.view = camera.GetView();
		rc.projection = camera.getProjection();

		// モデル描画
		this->model_shader->Render(dc, rc);

		// インスタンシング描画
		this->instance_model_shader->Render(dc, rc);
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
		case SceneDebug::ImGuiMode::System:
			ImGui::Text("System");
			break;
		default:
			break;
		}
	}
	ImGui::End();
}

#endif // _DEBUG