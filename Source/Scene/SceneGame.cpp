#include <imgui.h>
#include "SceneGame.h"
#include "Graphics/Graphics.h"
#include "System/Misc.h"
#include "Camera/CameraManager.h"
#include "Scene/SceneManager.h"


#include "Component/ModelComponent.h"
#include "Component/ModelShaderComponent.h"
#include "Component/InstancingModelComponent.h"
#include "Component/TransformComponent.h"
#include "Component/InstancingModelShaderComponent.h"
#include "Component/MovementComponent.h"
#include "Component/CameraComponent.h"
#include "Component/PlayerComponent.h"

#include "Model/AnimeTransitionJudgementDerived.h"

#include "ConstantManager.h"

void SceneGame::Initialize()
{
	Graphics* graphics = Graphics::Instance();
	ID3D11Device* device = graphics->GetDevice();
	// シェーダーの作成
	{
		instancing_model_shader = std::make_unique<InstancingModelShader>(device);
		model_shader = std::make_unique<ModelShader>(device);
	}

	// オブジェクト作成
	{
		// ステージ
		{
			auto stage = object_manager.Create();
			stage->SetName("Stage");
			stage->AddComponent<ModelComponent>(device, "Data/Model/ExampleStage/ExampleStage.mdl");
			auto transform = stage->AddComponent<Transform3DComponent>();
			transform->SetScale(DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));
			// シェーダー設定
			auto shader_component =
				stage->AddComponent<ModelShaderComponent>(model_shader.get());
		}

		// プレイヤー
		{
			auto player = object_manager.Create();
			player->SetName("Player");
			auto model = player->AddComponent<ModelComponent>(device, "Data/Model/Player/Player.mdl");
			model->PlayAnimation(PLAYER_ANIMATION::IDLE, true);
			model->SetAnimationState(PLAYER_ANIMATION::IDLE, true);
			model->AddAnimationTransition(PLAYER_ANIMATION::IDLE, PLAYER_ANIMATION::MOVE_FWD, std::make_unique<Judgement_Move>(player), 0.3f);
			
			model->SetAnimationState(PLAYER_ANIMATION::MOVE_FWD, true);
			model->AddAnimationTransition(PLAYER_ANIMATION::MOVE_FWD, PLAYER_ANIMATION::IDLE, std::make_unique<Judgement_Move>(player, true), 0.5f);
			
			auto transform = player->AddComponent<Transform3DComponent>();
			transform->SetScale(DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));
			transform->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
			player->AddComponent<MovementComponent>();
			player->AddComponent<PlayerComponent>();
			// シェーダー設定
			auto shader_component =
				player->AddComponent<ModelShaderComponent>(model_shader.get());
			// カメラ設定
			auto camera = player->AddComponent<CameraComponent>(CameraManager::Instance());
			camera->SetPerspectiveFov(
				DirectX::XMConvertToRadians(45.0f),
				graphics->GetScreenWidth() / graphics->GetScreenHeight(),
				0.1f,
				1000.0f
			);
			camera->SetRange(10.0f);
			camera->SetRotateX(0.4f);
			camera->SetMainCamera();
		}

		// 敵
		{
			for (int x = 0; x < 10; ++x)
			{
				for (int y = 0; y < 10; ++y)
				{

					const char* faile_name_arry[]
					{
						{"Data/Model/Jammo/Jammo.mdl"},
						{"Data/Model/Player/Player.mdl"},
					};

					auto enemy = object_manager.Create();
					auto model = enemy->AddComponent<InstancingModelComponent>(device, faile_name_arry[x % 2]);
					model->PlayAnimetion(x % 10, true);
					auto transform = enemy->AddComponent<Transform3DComponent>();

					float offset = 2.0f;

					DirectX::XMFLOAT3 pos =
					{
						offset * x,
						0.0f,
						offset * y ,
					};
					transform->SetPosition(pos);
					transform->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));

					// シェーダー設定
					auto shader_component =
						enemy->AddComponent<InstancingModelShaderComponent>(this->instancing_model_shader.get());
				}
			}
		}
	}
}

void SceneGame::Finalize()
{
}

void SceneGame::Update(float elapsed_time)
{
	object_manager.Update(elapsed_time);

	CameraManager::Instance()->Update(elapsed_time);
}

void SceneGame::Render()
{
	Graphics* graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics->GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics->GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics->GetDepthStencilView();

	FLOAT color[] = { 0.5f, 1.0f, 0.5f, 1.0f };
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 3Dモデルの描画
	{
		Graphics* graphics = Graphics::Instance();
		ID3D11DeviceContext* dc = graphics->GetDeviceContext();
		//Camera& camera = Camera::Intance();

		CameraManager* camera_manager = CameraManager::Instance();
		auto camera = camera_manager->GetMainCamera();
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

void SceneGame::DrawImGui()
{
	ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(ImVec2(300.0f, 400.0f), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("DebugMenu", nullptr, ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			// モード切り替え処理
			if (ImGui::BeginMenu("Mode"))
			{
				if (ImGui::MenuItem("Object"))
				{
					mode_index = SceneGame::ImGuiMode::Object;
				}
				if (ImGui::MenuItem("Shader"))
				{
					mode_index = SceneGame::ImGuiMode::Shader;
				}
				if (ImGui::MenuItem("Camera"))
				{
					mode_index = SceneGame::ImGuiMode::Camera;
				}
				if (ImGui::MenuItem("System"))
				{
					mode_index = SceneGame::ImGuiMode::System;
				}

				ImGui::EndMenu();
			}

			// カメラ切り替え
			if (ImGui::BeginMenu("Camera"))
			{
				bool& debug_flag = CameraManager::Instance()->debug_flag;

				std::string label = debug_flag ? "normal camera" : "debug camera";

				if (ImGui::MenuItem(label.c_str()))
				{
					debug_flag = !debug_flag;
					CameraManager::Instance()->SetDebugCamera();
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// 各モード表示
		switch (mode_index)
		{
		case SceneGame::ImGuiMode::Object:
			ImGui::Text("Object");
			object_manager.DrawDebugGUI();
			break;
		case SceneGame::ImGuiMode::Shader:
			ImGui::Text("System");
			DrawShaderImGui();
			break;
		case SceneGame::ImGuiMode::Camera:
			ImGui::Text("Camera");
			CameraManager::Instance()->DrawDebugGUI();
			break;
		case SceneGame::ImGuiMode::System:
			ImGui::Text("System");
			break;
		default:
			break;
		}
	}
	ImGui::End();
}

void SceneGame::DrawShaderImGui()
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