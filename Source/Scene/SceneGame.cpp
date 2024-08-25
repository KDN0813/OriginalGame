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

void SceneGame::Initialize()
{
	Graphics* graphics = Graphics::Instance();
	ID3D11Device* device = graphics->GetDevice();
	// �V�F�[�_�[�̍쐬
	{
		instancing_model_shader = std::make_unique<InstancingModelShader>(device);
		model_shader = std::make_unique<ModelShader>(device);
	}

	// �f�o�b�O�J�����쐬
	{
		auto debug_camera = object_manager.Create();
		debug_camera->SetName("debug_camera");

		auto camera_Cmp = debug_camera->AddComponent<CameraComponent>(CameraManager::Instance());
		camera_Cmp->SetPerspectiveFov(
			DirectX::XMConvertToRadians(45.0f),
			graphics->GetScreenWidth() / graphics->GetScreenHeight(),
			0.1f,
			1000.0f
		);
		camera_Cmp->SetRange(10.0f);
		camera_Cmp->SetRotateX(0.4f);
		camera_Cmp->SetMainCamera();
	}

	// �I�u�W�F�N�g�쐬
	{
		// �X�e�[�W
		{
			auto stage = object_manager.Create();
			stage->SetName("Stage");
			stage->AddComponent<ModelComponent>(device, "Data/Model/ExampleStage/ExampleStage.mdl");
			auto transform = stage->AddComponent<Transform3DComponent>();
			transform->SetScale(DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));
			// �V�F�[�_�[�ݒ�
			auto shader_component =
				stage->AddComponent<ModelShaderComponent>(model_shader.get());
		}

		// �v���C���[
		{
			auto player = object_manager.Create();
			player->SetName("Player");
			auto model = player->AddComponent<ModelComponent>(device, "Data/Model/Jammo/Jammo.mdl");
			model->PlayAnimation(0, true);
			auto transform = player->AddComponent<Transform3DComponent>();
			transform->SetScale(DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));
			transform->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
			player->AddComponent<MovementComponent>();
			player->AddComponent<PlayerComponent>();
			// �V�F�[�_�[�ݒ�
			auto shader_component =
				player->AddComponent<ModelShaderComponent>(model_shader.get());
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

	// 3D���f���̕`��
	{
		Graphics* graphics = Graphics::Instance();
		ID3D11DeviceContext* dc = graphics->GetDeviceContext();
		//Camera& camera = Camera::Intance();

		CameraManager* camera_manager = CameraManager::Instance();
		auto camera = camera_manager->GetMainCamera();
		RenderContext rc;
		rc.view = camera->GetViewTransform();
		rc.projection = camera->GetProjectionTransform();

		// ���f���`��
		this->model_shader->Render(dc, rc);

		// �C���X�^���V���O�`��
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
		// ���[�h�؂�ւ�����
		if (ImGui::BeginMenuBar())
		{
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
			ImGui::EndMenuBar();
		}

		// �e���[�h�\��
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