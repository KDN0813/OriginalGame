#include <imgui.h>
#include "SceneGame.h"
#include "Graphics/Graphics.h"
#include "System/Misc.h"
#include "Camera/CameraManager.h"
#include "Scene/SceneManager.h"

#ifdef _DEBUG
#include "Debug/DebugManager.h"
#include "Debug/DebugComponent.h"

#endif // _DEBUG

#include "ConstantManager.h"
#include "Input/GamePad.h"

#include "System/MyMath/MyMathf.h"
#include "Collision/Collision.h"

#include "Component/ModelComponent.h"
#include "Component/ModelShaderComponent.h"
#include "Component/InstancingModelComponent.h"
#include "Component/TransformComponent.h"
#include "Component/InstancingModelShaderComponent.h"
#include "Component/MovementComponent.h"
#include "Component/CameraComponent.h"
#include "Component/PlayerComponent.h"
#include "Component/GravityComponent.h"
#include "Component/ModelAnimationComponent.h"
#include "Component/EnemyComponent.h"
#include "Component/StateMachineComponent.h"
#include "Component/CircleCollisionComponent.h"
#include "Component/CameraControllerComponent.h"

#include "StateMachine/TransitionJudgementDerived.h"
#include "StateMachine/StateDerived.h"


void SceneGame::Initialize()
{
	Graphics* graphics = Graphics::Instance();
	ID3D11Device* device = graphics->GetDevice();
	// �V�F�[�_�[�̍쐬
	{
		instancing_model_shader = std::make_unique<InstancingModelShader>(device);
		model_shader = std::make_unique<ModelShader>(device);
	}

	// �I�u�W�F�N�g�쐬
	{
		// �X�e�[�W
		{
			auto stage = object_manager.Create();
			stage->SetName("Stage");
			stage->AddComponent<ModelComponent>(device, "Data/Model/Cube/Cube.mdl");
			auto transform = stage->AddComponent<Transform3DComponent>();
			transform->SetLocalScale(DirectX::XMFLOAT3(100.0f, 1.0f, 100.0f));
			transform->SetLocalPosition(DirectX::XMFLOAT3(0.0f, -0.5f, 0.0f));
			// �V�F�[�_�[�ݒ�
			auto shader_component =
				stage->AddComponent<ModelShaderComponent>(model_shader.get());

			// GameObject�ɐݒ�
			GameObject::Instance()->SetGameObject(GameObject::OBJECT_TYPE::STAGE, stage);
		}

		// �v���C���[
		{
			auto player = object_manager.Create();
			player->SetName("Player");
			auto model = player->AddComponent<ModelComponent>(device, "Data/Model/Player/Player.mdl");
			auto model_animation = player->AddComponent<ModelAnimationComponent>(device, "Data/Model/Player/Player.mdl");
			//auto model = player->AddComponent<AnimatedInstancedModelComponent>(device, "Data/Model/Player/Player.mdl");
			// �A�j���[�V�����ݒ�
			{
				// �ҋ@
				model_animation->PlayAnimation(PlayerCT::ANIMATION::IDLE, true);
				model_animation->SetAnimationState(PlayerCT::ANIMATION::IDLE, true);
				model_animation->AddAnimationTransition(PlayerCT::ANIMATION::IDLE, PlayerCT::ANIMATION::MOVE_FWD, std::make_unique<Judgement_Move>(player),0.2f);
				model_animation->AddAnimationTransition(PlayerCT::ANIMATION::IDLE, PlayerCT::ANIMATION::ATTACK01, std::make_unique<Judgement_ButtonDown>(player, GamePad::BTN_X), 0.2f);

				// �O���ړ�
				model_animation->SetAnimationState(PlayerCT::ANIMATION::MOVE_FWD, true);
				model_animation->AddAnimationTransition(PlayerCT::ANIMATION::MOVE_FWD, PlayerCT::ANIMATION::IDLE, std::make_unique<Judgement_Move>(player, true), 0.2f);
				model_animation->AddAnimationTransition(PlayerCT::ANIMATION::MOVE_FWD, PlayerCT::ANIMATION::ATTACK01, std::make_unique<Judgement_ButtonDown>(player, GamePad::BTN_X), 0.2f);

				// �U��01
				model_animation->SetAnimationState(PlayerCT::ANIMATION::ATTACK01, false);
				model_animation->AddAnimationTransition(PlayerCT::ANIMATION::ATTACK01, PlayerCT::ANIMATION::IDLE, std::make_unique<Judgement_TransitionReady>(player, false, true), 0.2f);
			}
			auto state_machine = player->AddComponent<StateMachineComponent>();
			{
				auto idle_state = state_machine->RegisterState<IdelState>();
				idle_state->AddStateTransition(std::make_unique<StateTransitionInfo>("AttackState", std::make_unique<Judgement_ButtonDown>(player, GamePad::BTN_X)), StateBase::JudgementUpdatePhase::PostUpdate);
				auto attack_state = state_machine->RegisterState<AttackState>();
				//attack_state->AddStateTransition(std::make_unique<StateTransitionInfo>("IdelState", std::make_unique<Judgement_ButtonDown>(player, GamePad::BTN_Y)), StateBase::JudgementUpdatePhase::PostUpdate);

				state_machine->SetDefaultState("IdelState");
			}

			auto transform = player->AddComponent<Transform3DComponent>();
			transform->SetLocalScale(DirectX::XMFLOAT3(0.008f, 0.008f, 0.008f));
			auto movement = player->AddComponent<MovementComponent>();
			movement->SetIsStageRaycas(true);
			player->AddComponent<PlayerComponent>();
			// �V�F�[�_�[�ݒ�
			auto shader_component =
				player->AddComponent<ModelShaderComponent>(model_shader.get());
			//auto shader_component =
			//player->AddComponent<InstancingModelShaderComponent>(this->instancing_model_shader.get());
			// �J�����ݒ�
			{
				CameraComponent::CameraParam camera_param{};
				camera_param.fovY = DirectX::XMConvertToRadians(45.0f);
				camera_param.aspect = graphics->GetScreenWidth() / graphics->GetScreenHeight();
				camera_param.nearZ = 0.1f;
				camera_param.farZ = 1000.0f;
				camera_param.range = 10.0f;
				camera_param.rotateX = 0.4f;

				auto camera = player->AddComponent<CameraComponent>(camera_param, CameraManager::Instance());
				camera->SetMainCamera();

				player->AddComponent<CameraControllerGamepad>();
			}
			// �d��
			player->AddComponent<GravityComponent>();
			// �~�̃R���C�_�[
			{
				auto collision = player->AddComponent<CircleCollisionComponent>();
				collision->SetCollisionType(COLLISION_TYPE::ATTACKER);
				collision->SetSelfType(OBJECT_TYPE::PLAYER);
				collision->SetTargetType(OBJECT_TYPE::ENEMY);
				collision->SetIsActive(false);

				CollisionManager::Instance()->GetCircleCollider()->AddCircle(collision);
			}

			// �q�I�u�W�F�N�g�ݒ�
			{
				std::shared_ptr<Object> object = player->AddChildren();
				object->SetName("player child");
				object->AddComponent<ModelComponent>(device, "Data/Model/Jammo/Jammo.mdl");
				auto transform = object->AddComponent<Transform3DComponent>();
				transform->SetLocalScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
				transform->SetLocalPosition(DirectX::XMFLOAT3(100.0f, 0.0f, 0.0f));
				auto movement = object->AddComponent<MovementComponent>();
				// �V�F�[�_�[�ݒ�
				auto shader_component =
					object->AddComponent<ModelShaderComponent>(model_shader.get());
			}

			// GameObject�ɐݒ�
			GameObject::Instance()->SetGameObject(GameObject::OBJECT_TYPE::PLAYER, player);
		}

		// �G
		{
			float territory_range = 45.0f;
			for (int i = 0; i < 1; ++i)
			{
				auto enemy = object_manager.Create();
				auto transform = enemy->AddComponent<Transform3DComponent>();
				auto enemy_component = enemy->AddComponent<EnemyComponent>();
				auto movement = enemy->AddComponent<MovementComponent>();
				// �A�j���[�V�����ݒ�
				auto model = enemy->AddComponent<AnimatedInstancedModelComponent>(device, "Data/Model/ChestMonster/ChestMonster.mdl");
				model->PlayAnimation(EnemyCT::ANIMATION::MOVE_FWD, true);
				{
					model->SetAnimationState(EnemyCT::ANIMATION::MOVE_FWD, true);
					model->AddAnimationTransition(EnemyCT::ANIMATION::MOVE_FWD, EnemyCT::ANIMATION::IDLE_BATTLE, std::make_unique<Judgement_IsAtTarget>(enemy));
					model->SetAnimationState(EnemyCT::ANIMATION::IDLE_BATTLE, true);
					model->AddAnimationTransition(EnemyCT::ANIMATION::IDLE_BATTLE, EnemyCT::ANIMATION::MOVE_FWD, std::make_unique<Judgement_IdleFinished>(enemy));
				}
				// �X�e�[�g�ݒ�
				auto state_machine = enemy->AddComponent<StateMachineComponent>();
				{
					auto idle_state = state_machine->RegisterState<IdelState>();
					idle_state->AddStateTransition(std::make_unique<StateTransitionInfo>("WanderState", std::make_unique<Judgement_IdleFinished>(enemy)), StateBase::JudgementUpdatePhase::PostUpdate);
					auto wander_state = state_machine->RegisterState<WanderState>();
					wander_state->AddStateTransition(std::make_unique<StateTransitionInfo>("IdelState", std::make_unique<Judgement_IsAtTarget>(enemy)), StateBase::JudgementUpdatePhase::PostUpdate);

					state_machine->SetDefaultState("WanderState");
				}
				// �~�̃R���C�_�[
				{
					auto collision = enemy->AddComponent<CircleCollisionComponent>();
					collision->SetCollisionType(COLLISION_TYPE::DEFENDER);
					collision->SetSelfType(OBJECT_TYPE::ENEMY);
					collision->SetTargetType(OBJECT_TYPE::PLAYER);

					CollisionManager::Instance()->GetCircleCollider()->AddCircle(collision);
				}

				float offset = 2.0f;

				float theta = MyMathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
				float range = MyMathf::RandomRange(0.0f, territory_range);
				DirectX::XMFLOAT3 pos =
				{
					sinf(theta)* range,
					0.0f,
					cosf(theta)* range ,
				};
				transform->SetLocalPosition(pos);
				transform->SetLocalScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));

				// �V�F�[�_�[�ݒ�
				auto shader_component =
					enemy->AddComponent<InstancingModelShaderComponent>(this->instancing_model_shader.get());
			}
		}

#ifdef _DEBUG	// �f�o�b�O�pobject
		auto debug_camera = object_manager.Create();
		debug_camera->SetName("Debug Camera");
		debug_camera->AddComponent<Transform3DComponent>();
		debug_camera->AddComponent<CameraControllerDebug>();
		
		// �J�����ݒ�
		{
			CameraComponent::CameraParam camera_param{};
			camera_param.fovY = DirectX::XMConvertToRadians(45.0f);
			camera_param.aspect = graphics->GetScreenWidth() / graphics->GetScreenHeight();
			camera_param.nearZ = 0.1f;
			camera_param.farZ = 1000.0f;
			camera_param.range = 10.0f;
			camera_param.rotateX = 0.4f;

			auto debug_camera_component = debug_camera->AddComponent<CameraComponent>(camera_param, CameraManager::Instance());
			CameraManager::Instance()->SetDebugCamera(debug_camera_component.get());
		}
#endif // _DEBUG
	}
}

void SceneGame::Finalize()
{
}

void SceneGame::Update(float elapsed_time)
{
	object_manager.Update(elapsed_time);

	CameraManager::Instance()->Update(elapsed_time);

	CollisionManager::Instance()->Update();
}

void SceneGame::Render()
{
	Graphics* graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics->GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics->GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics->GetDepthStencilView();

	CameraManager* camera_manager = CameraManager::Instance();
	auto camera = camera_manager->GetMainCamera();
	RenderContext rc;
	rc.view = camera->GetViewTransform();
	rc.projection = camera->GetProjectionTransform();

	FLOAT color[] = { 0.5f, 1.0f, 0.5f, 1.0f };
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 3D���f���̕`��
	{
		// ���f���`��
		this->model_shader->Render(dc, rc);

		// �C���X�^���V���O�`��
		this->instancing_model_shader->Render(dc, rc);
	}

#ifdef _DEBUG
	// 3D�f�o�b�O�`��
	{
		object_manager.DrawDebugPrimitive();

		DebugManager::Instance()->GetDebugPrimitiveRenderer()->Render(dc, rc.view, rc.projection);;
	}

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
			// ���[�h�؂�ւ�����
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
				if (ImGui::MenuItem("Collison"))
				{
					mode_index = SceneGame::ImGuiMode::Collison;
				}
				if (ImGui::MenuItem("System"))
				{
					mode_index = SceneGame::ImGuiMode::System;
				}

				ImGui::EndMenu();
			}

			// �J�����؂�ւ�
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
		case SceneGame::ImGuiMode::Collison:
			ImGui::Text("Collison");
			CollisionManager::Instance()->DrawDebugGUI();
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