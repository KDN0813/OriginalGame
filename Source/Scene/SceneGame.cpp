#include <imgui.h>
#include "SceneGame.h"
#include "Graphics/Graphics.h"
#include "System/Misc.h"
#include "Camera/CameraManager.h"

// �V�[��
#include "Scene/SceneManager.h"
#include "Scene/SceneTitle.h"
#include "Scene/SceneResult.h"
#include "Scene/SceneLoading.h"

#ifdef _DEBUG
#include "Debug/DebugManager.h"
#include "Debug/DebugComponent.h"

#endif // _DEBUG

#include "ConstantManager.h"
#include "Input/GamePad.h"
#include "System/GameData.h"

#include "System/MyMath/MyMathf.h"
#include "Collision/Collision.h"
#include "Audio/Audio.h"

#include "Component/ModelComponent.h"
#include "Component/ModelShaderComponent.h"
#include "Component/InstancingModelComponent.h"
#include "Component/TransformComponent.h"
#include "Component/InstancingModelShaderComponent.h"
#include "Component/MovementComponent.h"
#include "Component/CameraComponent_ver2.h"
#include "Component/PlayerComponent.h"
#include "Component/GravityComponent.h"
#include "Component/ModelAnimationComponent.h"
#include "Component/EnemyComponent.h"
#include "Component/StateMachineComponent.h"
#include "Component/CircleCollisionComponent.h"
#include "Component/CameraControllerComponent.h"
#include "Component/SpriteComponent.h"

#include "StateMachine/TransitionJudgementDerived.h"
#include "StateMachine/StateDerived.h"


void SceneGame::Initialize()
{
	Graphics* graphics = Graphics::Instance();
	// �V�F�[�_�[�̍쐬
	{
		instancing_model_shader = std::make_unique<InstancingModelShader>();
		model_shader = std::make_unique<ModelShader>();
		sprite_shader = std::make_unique<SpriteShader>();
	}

	// �I�u�W�F�N�g�쐬
	{
		// �X�e�[�W
		{
			auto stage = object_manager.Create();
			stage->SetName("Stage");
			stage->AddComponent<ModelComponent>("Data/Debug/Model/Cube/Cube.mdl");
			// �g�����X�t�H�[���ݒ�
			{
				Transform3DComponent::Transform3DParam param{};
				param.local_scale = DirectX::XMFLOAT3(100.0f, 1.0f, 100.0f);
				param.local_position = DirectX::XMFLOAT3(0.0f, -0.5f, 0.0f);
				auto transform = stage->AddComponent<Transform3DComponent>(param);
			}
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
			auto model = player->AddComponent<ModelComponent>("Data/Model/Player/Player.mdl");
			//auto model = player->AddComponent<AnimatedInstancedModelComponent>(device, "Data/Model/Player/Player.mdl");
			// �A�j���[�V�����ݒ�
			{
				ModelAnimationComponent::AnimationParam param{};
				param.current_animation_index = PlayerCT::ANIMATION::IDLE;
				param.animation_loop_flag = true;

				auto model_animation = player->AddComponent<ModelAnimationComponent>(param , "Data/Model/Player/Player.mdl");
				// �ҋ@
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

			// �g�����X�t�H�[���ݒ�
			{
				Transform3DComponent::Transform3DParam param{};
				param.local_scale = DirectX::XMFLOAT3(0.008f, 0.008f, 0.008f);
				auto transform = player->AddComponent<Transform3DComponent>(param);
			}
			// ���[�u�����g�ݒ�
			{
				MovementComponent::MovementParam param{};
				param.is_stage_raycas = true;
				auto movement = player->AddComponent<MovementComponent>(param);
			}
			{
				player->AddComponent<PlayerComponent>(PlayerComponent::PlayerParam());
			}
			// �V�F�[�_�[�ݒ�
			auto shader_component =
				player->AddComponent<ModelShaderComponent>(model_shader.get());
			//auto shader_component =
			//player->AddComponent<InstancingModelShaderComponent>(this->instancing_model_shader.get());
			// �J�����ݒ�
			{
				CameraManager* camera_manager = CameraManager::Instance();
				if (camera_manager)
				{
					auto camera = player->AddComponent<CameraComponent>(camera_manager->GetCamera(CAMERA_TYPE::MAIN));
				}

			}
			// �J�����R���g���[���[�ݒ�
			{
				CameraControllerGamepad::CameraControllerParam param{};
				player->AddComponent<CameraControllerGamepad>(param);
			}
			// �d��
			{
				player->AddComponent<GravityComponent>(GravityComponent::GravityParam());
			}
			// �~�̃R���C�_�[
			{
				CircleCollisionComponent::CollisionParam param{};
				param.collision_type = COLLISION_TYPE::ATTACKER;
				param.self_type = OBJECT_TYPE::PLAYER;
				param.target_type = OBJECT_TYPE::ENEMY;
				param.default_active_flag = false;
				auto collision = player->AddComponent<CircleCollisionComponent>(param);

				CollisionManager::Instance()->GetCircleCollider()->AddCircle(collision);
			}

			// �q�I�u�W�F�N�g�ݒ�
			{
				std::shared_ptr<Object> object = player->AddChildren();
				object->SetName("player child");
				object->AddComponent<ModelComponent>("Data/Debug/Model/Jammo/Jammo.mdl");

				// �g�����X�t�H�[���ݒ�
				{
					Transform3DComponent::Transform3DParam param{};
					param.local_position = DirectX::XMFLOAT3(100.0f, 0.0f, 0.0f);
					param.local_scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

					auto transform = object->AddComponent<Transform3DComponent>(param);
				}
				// ���[�u�����g�ݒ�
				{
					auto movement = object->AddComponent<MovementComponent>(MovementComponent::MovementParam());
				}
				// �V�F�[�_�[�ݒ�
				auto shader_component =
					object->AddComponent<ModelShaderComponent>(model_shader.get());
			}

			// GameObject�ɐݒ�
			GameObject::Instance()->SetGameObject(GameObject::OBJECT_TYPE::PLAYER, player);
		}


		// �f�X�J����
		{
			auto death_camera = object_manager.Create();
			CameraManager* camera_manager = CameraManager::Instance();
			if (camera_manager)
			{
				auto camera = death_camera->AddComponent<CameraComponent>(camera_manager->GetCamera(CAMERA_TYPE::DEATH));
			}
		}

		// �G
		{
			float territory_range = 45.0f;
			for (int i = 0; i < 1000; ++i)
			{
				auto enemy = object_manager.Create();
				// �G�l�~�[�R���|�[�l���g�ݒ�
				{
					EnemyComponent::EnemyParam param{};
					auto enemy_component = enemy->AddComponent<EnemyComponent>(param);
				}
				// ���[�u�����g�ݒ�
				{
					auto movement = enemy->AddComponent<MovementComponent>(MovementComponent::MovementParam());
				}
				// �A�j���[�V�����ݒ�
				{
					AnimatedInstancedModelComponent::InstancedModelParam param;
					param.anime_index = EnemyCT::ANIMATION::IDLE_BATTLE;
					param.anime_loop = true;
					param.anime_play = true;

					auto model = enemy->AddComponent<AnimatedInstancedModelComponent>(param, "Data/Model/ChestMonster/ChestMonster.mdl");
					{
						model->SetAnimationState(EnemyCT::ANIMATION::MOVE_FWD, true);
						model->AddAnimationTransition(EnemyCT::ANIMATION::MOVE_FWD, EnemyCT::ANIMATION::IDLE_BATTLE, std::make_unique<Judgement_IsAtTarget>(enemy));
						model->SetAnimationState(EnemyCT::ANIMATION::IDLE_BATTLE, true);
						model->AddAnimationTransition(EnemyCT::ANIMATION::IDLE_BATTLE, EnemyCT::ANIMATION::MOVE_FWD, std::make_unique<Judgement_IdleFinished>(enemy));
					}
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
					CircleCollisionComponent::CollisionParam param{};
					param.collision_type = COLLISION_TYPE::DEFENDER;
					param.self_type = OBJECT_TYPE::ENEMY;
					param.target_type = OBJECT_TYPE::PLAYER;
					auto collision = enemy->AddComponent<CircleCollisionComponent>(param);
					CollisionManager::Instance()->GetCircleCollider()->AddCircle(collision);
				}
				// �g�����X�t�H�[���ݒ�
				{
					float offset = 2.0f;
					float theta = MyMathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
					float range = MyMathf::RandomRange(0.0f, territory_range);

					Transform3DComponent::Transform3DParam param{};
					param.local_position  = 
					{
						sinf(theta) * range,
						0.0f,
						cosf(theta) * range ,
					};
					param.local_scale = DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f);

					auto transform = enemy->AddComponent<Transform3DComponent>(param);
				}

				// �V�F�[�_�[�ݒ�
				auto shader_component =
					enemy->AddComponent<InstancingModelShaderComponent>(this->instancing_model_shader.get());
			}
		}

#ifdef _DEBUG	// �f�o�b�O�pobject
		AudioParam param{};
		param.volume = 0.3f;
		param.loop = true;
		param.filename = "Data/Debug/Audio/BGM.wav";
		Audio::Instance()->Play(param);
#endif // _DEBUG
	}
}

void SceneGame::Finalize()
{
	// �I�[�f�B�I�̃��Z�b�g
	Audio* audio = Audio::Instance();
	if (audio)
	{
		audio->AllClear();
	}
}

void SceneGame::Update(float elapsed_time)
{
	object_manager.Update(elapsed_time);

	CameraManager::Instance()->Update(elapsed_time);

	CollisionManager::Instance()->Update();

	Audio::Instance()->Update();

#ifdef _DEBUG
	// �X�y�[�X�L�[�ŃQ�[����ʂɑJ��(��)
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		AudioParam param{};
		param.filename = "Data/Debug/Audio/SE.wav";
		Audio::Instance()->Play(param);
	}
#endif // DEBUG

	// �Q�[����Ԃ���������
	ProcessGameState();
}

void SceneGame::Render()
{
	Graphics* graphics = Graphics::Instance();
	graphics->PrepareRenderTargets(DirectX::XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f));

	// 3D���f���̕`��
	{
		// ���f���`��
		this->model_shader->Render();

		// �C���X�^���V���O�`��
		this->instancing_model_shader->Render();
	}

#ifdef _DEBUG
	// 3D�f�o�b�O�`��
	{
		object_manager.DrawDebugPrimitive();

		DebugManager::Instance()->GetDebugPrimitiveRenderer()->Render();
	}
#endif // _DEBUG

	// 2D�X�v���C�g�`��
	{
		sprite_shader->Render();
	}
}

void SceneGame::ReStart()
{
	object_manager.ReStart();

	// �Q�[����Ԃ��f�t�H���g�ɐݒ�
	GameData* game_data = GameData::Instance();
	if (game_data)
	{
		game_data->SetGameStatus(GameData::GameStatus::DEFAULT);
	}
}

void SceneGame::ProcessGameState()
{
	GameData* game_data = GameData::Instance();
	if (!game_data) return;

	// �Q�[����Ԃ��擾
	const GameData::GameStatus& game_status = game_data->GetGameStatus();

	switch (game_status)
	{
	case GameData::GameStatus::DEFEAT:	// �v���C���[�̔s�k
	{
		SceneManager* scene_manager = SceneManager::Instance();
		if (!scene_manager) return;

		scene_manager->ChangeScene(new SceneResult);
		break;
	}
	case GameData::GameStatus::VICTORY:	// �v���C���[�̏���
	{
		SceneManager* scene_manager = SceneManager::Instance();
		if (!scene_manager) return;

		scene_manager->ChangeScene(new SceneLoading(new SceneResult));
		break;
	}
	case GameData::GameStatus::RETURN_TO_TITLE: // �^�C�g���ɖ߂�
	{
		SceneManager* scene_manager = SceneManager::Instance();
		if (!scene_manager) return;

		scene_manager->ChangeScene(new SceneLoading(new SceneTitle));
		break;
	}
	case GameData::GameStatus::RESTART:	// ���X�^�[�g
	{
		ReStart();
		break;
	}
	default:break;
	}
}

#ifdef _DEBUG

void SceneGame::DebugDrawGUI()
{
	this->object_manager.DrawDebugGUI();

	// �V�F�[�_�[
	DrawShaderImGui();

	// �R���W����
	this->collision_manager.DrawDebugGUI();
}

void SceneGame::DrawShaderImGui()
{
	if (ImGui::Begin("Sahder"))
	{
		if (ImGui::CollapsingHeader(model_shader->GetName()))
		{
			model_shader->DrawDebugGUI();
		}
		if (ImGui::CollapsingHeader(instancing_model_shader->GetName()))
		{
			instancing_model_shader->DrawDebugGUI();
		}
		if (ImGui::CollapsingHeader(sprite_shader->GetName()))
		{
			sprite_shader->DrawDebugGUI();
		}
	}
	ImGui::End();
}

#endif // _DEBUG