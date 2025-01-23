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
	// �V�F�[�_�[�̍쐬
	{
		model_shader = std::make_unique<ModelShader>();
	}
	// �p�[�e�B�N���V�X�e���̃e�N�X�`�����[�h
	{
		particle_system.LoadTexture("Data/Effect/Texture/Line01.png");
	}

	// �I�u�W�F�N�g�쐬
	{
		auto stage = object_manager.Create("Stage");
		stage->AddComponent<ModelComponent>("Data/Model/Floor/StageFloor.mdl");
		// �g�����X�t�H�[���ݒ�
		{
			Transform3DComponent::Transform3DParam param{};
			param.local_scale = DirectX::XMFLOAT3(600.0f, 1.0f, 600.0f);
			param.local_position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			auto transform = stage->AddComponent<Transform3DComponent>(param);
		}
		// �V�F�[�_�[�ݒ�
		auto shader_component =
			stage->AddComponent<ModelShaderComponent>(model_shader.get());
	}
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

	if (ParticleSystem::Instance particle_system = ParticleSystem::GetInstance(); particle_system.Get())
	{
		particle_system->SetElapsedTime(elapsed_time);
	}
}

void SceneDebug::Render()
{
	// ��ʂ̃N���A
	{
		Graphics::Instance graphics = Graphics::GetInstance();
		if (!graphics.Get()) return;
		graphics->PrepareRenderTargets();
	}

	// 3D���f���̕`��
	{
		// ���f���`��
		this->model_shader->Render();
	}

#ifdef _DEBUG
	DebugDrawGUI();
#endif // _DEBUG

	// 2D�X�v���C�g�`��
	{
		if (ParticleSystem::Instance particle_system = ParticleSystem::GetInstance(); particle_system.Get())
		{
			particle_system->Update();
			particle_system->Render();
		}
	}
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
	//if (ImGui::CollapsingHeader(instancing_model_shader->GetName(), ImGuiTreeNodeFlags_DefaultOpen))
	//{
	//	instancing_model_shader->DrawDebugGUI();
	//}
}

#endif // _DEBUG