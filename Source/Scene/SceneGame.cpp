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
#include "Component/InstancedModelWithAnimationComponent.h"
#include "Component/TransformComponent.h"
#include "Component/InstancingModelShaderComponent.h"
#include "Component/MovementComponent.h"
#include "Component/CameraComponent_ver2.h"
#include "Component/PlayerComponent.h"
#include "Component/GravityComponent.h"
#include "Component/ModelAnimationComponent.h"
#include "Component/ModelAnimationControlComponent.h"
#include "Component/EnemyComponent.h"
#include "Component/CircleCollisionComponent.h"
#include "Component/CameraControllerComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/CharacterComponent.h"
#include "Component/StateMachineComponent.h"

#pragma region LegacyComponent(���R���|�[�l���g)
#include "Component/LegacyStateMachineComponent.h"
#pragma endregion LegacyComponent(���R���|�[�l���g)

#include "StateMachine/LegacyTransitionJudgementDerived.h"
#include "StateMachine/LegacyStateDerived.h"
#include "StateMachine/StateDerived.h"


void SceneGame::Initialize()
{
	// �V�F�[�_�[�̍쐬
	{
		instancing_model_shader = std::make_unique<InstancingModelShader>();
		model_shader = std::make_unique<ModelShader>();
		sprite_shader = std::make_unique<SpriteShader>();
		particle_system = std::make_unique<ParticleSystem>("Data/Effect/Texture/Line01.png");
	}

	// �I�u�W�F�N�g�쐬
	{
		// �X�e�[�W
		{
			auto stage = object_manager.Create("Stage");
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

			if (GameObject::Instance game_object = GameObject::GetInstance(); game_object.Get())
			{
				game_object->SetStage(stage);
			}
		}

		// �v���C���[
		{
			auto player = object_manager.Create("Player");

			// �R���W�����ɐݒ肷��R���|�[�l���g�͎��O�ɍ쐬���Ă���
			std::shared_ptr<PlayerComponent> player_component;// �v���C���[�R���|�[�l���g

			// ���f���ݒ�
			{
				auto model = player->AddComponent<ModelComponent>("Data/Model/Player/Player.mdl");
			}
			//auto model = player->AddComponent<AnimatedInstancedModelComponent>(device, "Data/Model/Player/Player.mdl");
			// �A�j���[�V�����ݒ�
			{
				auto model_animation = player->AddComponent<ModelAnimationControlComponent>("Data/Model/Player/Player.mdl");
			}
			// �X�e�[�g�}�V���ݒ�
			auto state_machine = player->AddComponent<StateMachineComponent>();
			{
				// �X�e�[�g�̒ǉ�
				state_machine->RegisterState<PlayerIdleState>();
				state_machine->RegisterState<PlayerMoveState>();
				state_machine->RegisterState<PlayerAttackState>();

				state_machine->SetDefaultState("PlayerIdleState");	// �f�t�H���g�X�e�[�g�̐ݒ�
			}

			// �g�����X�t�H�[���ݒ�
			{
				Transform3DComponent::Transform3DParam param{};
				param.local_scale = DirectX::XMFLOAT3(0.015f, 0.015f, 0.015f);
				auto transform = player->AddComponent<Transform3DComponent>(param);
			}
			// ���[�u�����g�ݒ�
			{
				MovementComponent::MovementParam param{};
				param.is_stage_raycas = true;
				auto movement = player->AddComponent<MovementComponent>(param);
			}
			// �v���C���[�R���|�[�l���g�쐬
			{
				player_component = player->AddComponent<PlayerComponent>(PlayerComponent::PlayerParam());
			}
			// �V�F�[�_�[�ݒ�
			{
				auto shader_component =
					player->AddComponent<ModelShaderComponent>(model_shader.get());
			}
			// �J�����ݒ�
			{
				if (CameraManager::Instance camera_manager = CameraManager::GetInstance(); camera_manager.Get())
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
			// �L�����N�^�[�X�e�[�^�X
			{
				CharacterComponent::CharacterParam param{};
				param.max_hp = 100;
				param.hp = 50;
				player->AddComponent<CharacterComponent>(param);
			}


			// �q�I�u�W�F�N�g
			{
				// �v���C���[�̍U������p�I�u�W�F�N�g
				{
					std::shared_ptr<Object> player_attack_object = player->CreateChildObject();
					player_attack_object->SetName("AttackObject");
					// �g�����X�t�H�[���ݒ�
					{
						Transform3DComponent::Transform3DParam param{};
						param.local_position = DirectX::XMFLOAT3(0.0f, 0.0f, 100.0f);
						param.local_scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

						auto child_transform = player_attack_object->AddComponent<Transform3DComponent>(param);
					}
					// ���[�u�����g�ݒ�
					{
						auto child_movement = player_attack_object->AddComponent<MovementComponent>(MovementComponent::MovementParam());
					}
					// �~�̃R���C�_�[
					{
						CircleCollisionComponent::CollisionParam param{};
						param.collision_type = COLLISION_OBJECT_TYPE::PLAYER;
						param.default_active_flag = false;
						auto child_collision = player_attack_object->AddComponent<CircleCollisionComponent>(param);

						// �ڐG����������R���|�[�l���g�̒ǉ�
						{
							child_collision->AddCollisionEntercomponent(player_component);
						}
					}
				}
			}

			// GameObject�ɐݒ�
			if (GameObject::Instance game_object = GameObject::GetInstance(); game_object.Get())
			{
				game_object->SetPlayer(player);
			}
		}


		// �f�X�J����
		{
			auto death_camera = object_manager.Create("Death Camera");
			if (CameraManager::Instance camera_manager = CameraManager::GetInstance(); camera_manager.Get())
			{
				auto camera = death_camera->AddComponent<CameraComponent>(camera_manager->GetCamera(CAMERA_TYPE::DEATH));
			}
		}
		// �G
		{
			float territory_range = 45.0f;
			for (int i = 0; i < 0; ++i)
			{
				auto enemy = object_manager.Create();

				// �R���W�����ɐݒ肷��R���|�[�l���g�͎��O�ɍ쐬���Ă���
				std::shared_ptr<EnemyComponent> enemy_component;

				// �G�l�~�[�R���|�[�l���g�ݒ�
				{
					EnemyComponent::EnemyParam param{};
					enemy_component = enemy->AddComponent<EnemyComponent>(param);
				}
				// ���[�u�����g�ݒ�
				{
					auto movement = enemy->AddComponent<MovementComponent>(MovementComponent::MovementParam());
				}
				// ���f���ݒ�
				{
					InstancedModelWithAnimationComponent::InstancedModelParam param;
					param.anime_index = EnemyCT::ANIMATION::IDLE_BATTLE;
					param.anime_loop = true;
					param.anime_play = true;

					auto model = enemy->AddComponent<InstancedModelWithAnimationComponent>(param, "Data/Model/ChestMonster/ChestMonster.mdl");
				}
				// �g�����X�t�H�[���ݒ�
				{
					float offset = 2.0f;
					float theta = MyMathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
					float range = MyMathf::RandomRange(0.0f, territory_range);

					Transform3DComponent::Transform3DParam param{};
					param.local_position =
					{
#if 0	// �����l�Œ�
						0.0f,
						0.0f,
						0.0f,
#else
						sinf(theta) * range,
						0.0f,
						cosf(theta) * range ,
#endif
					};
					param.local_scale = DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f);

					auto transform = enemy->AddComponent<Transform3DComponent>(param);
				}

				// �L�����N�^�[�X�e�[�^�X
				{
					CharacterComponent::CharacterParam param{};
					param.max_hp = 10;
					param.hp = 2;
					enemy->AddComponent<CharacterComponent>(param);
				}

				// �V�F�[�_�[�ݒ�
				auto shader_component =
					enemy->AddComponent<InstancingModelShaderComponent>(this->instancing_model_shader.get());

				// �~�̃R���C�_�[
				{
					CircleCollisionComponent::CollisionParam param{};
					param.collision_type = COLLISION_OBJECT_TYPE::ENEMY;
					auto collision = enemy->AddComponent<CircleCollisionComponent>(param);
					collision->AddCollisionEntercomponent(enemy_component);
				}

				if (GameObject::Instance game_object = GameObject::GetInstance(); game_object.Get())
				{
					game_object->SetEnemy(enemy);
				}
			}
		}
		
		#ifdef _DEBUG	// �f�o�b�O�pobject
				if (Audio::Instance audio = Audio::GetInstance(); audio.Get())
				{
					AudioParam param{};
					param.volume = 0.3f;
					param.loop = true;
					param.filename = "Data/Debug/Audio/BGM.wav";
					audio->Play(param);
				}
		#endif // _DEBUG
	}
}

void SceneGame::Finalize()
{
	// �I�[�f�B�I�̃��Z�b�g
	if (Audio::Instance audio = Audio::GetInstance(); audio.Get())
	{
		audio->AllClear();
	}
}

void SceneGame::Update(float elapsed_time)
{
	if (GameData::Instance game_data = GameData::GetInstance() ; game_data.Get())
	{
		game_data->UpdateGameEndTimer(elapsed_time);
		if (game_data->IsTimeUp())
		{
			// �������Ԃ��߂��Ă�����Q�[����Ԃ�ύX
			game_data->SetGameStatus(GameData::GameStatus::VICTORY);
		}
	}

	object_manager.Update(elapsed_time);

	if (CameraManager::Instance camera_manager = CameraManager::GetInstance(); camera_manager.Get())
	{
		camera_manager->Update(elapsed_time);
	}

	if (Audio::Instance audio = Audio::GetInstance(); audio.Get())
	{
		audio->Update();
	}

	if (CircleCollisionManager::Instance collision_manager = CircleCollisionManager::GetInstance(); collision_manager.Get())
	{
		collision_manager->VsEnemy();
	}

#ifdef _DEBUG
	// �X�y�[�X�L�[��SE�Đ�
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (Audio::Instance audio = Audio::GetInstance(); audio.Get())
		{
			AudioParam param{};
			param.filename = "Data/Debug/Audio/SE.wav";
			audio->Play(param);
		}

		particle_system->Set(
			10.0f,
			DirectX::XMFLOAT3(0.0f, 3.0f, 0.0f),
			DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT2(10.0f, 10.0f)
		);
	}
#endif // DEBUG

	particle_system->Update(elapsed_time);

	// �Q�[����Ԃ���������
	ProcessGameState();
}

void SceneGame::Render()
{
	Graphics::Instance graphics = Graphics::GetInstance();
	if (graphics.Get())
	{
		graphics->PrepareRenderTargets(DirectX::XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f));
	}

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

		if (DebugManager::Instance debug_manager = DebugManager::GetInstance(); debug_manager.Get())
		{
			debug_manager->GetDebugPrimitiveRenderer()->Render();
		}
	}
#endif // _DEBUG

	// 2D�X�v���C�g�`��
	{
		if (particle_system.get())
		{
			particle_system->Render();
		}

		sprite_shader->Render();
	}
}

void SceneGame::ReStart()
{
	object_manager.ReStart();

	// �Q�[����Ԃ��f�t�H���g�ɐݒ�
	if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
	{
		game_data->SetGameStatus(GameData::GameStatus::DEFAULT);
	}

	if (CameraManager::Instance camera_manager = CameraManager::GetInstance(); camera_manager.Get())
	{
		camera_manager->Reset();
	}
}

void SceneGame::ProcessGameState()
{
	GameData::Instance game_data = GameData::GetInstance();
	if (!game_data.Get()) return;

	// �Q�[����Ԃ��擾
	const GameData::GameStatus& game_status = game_data->GetGameStatus();

	switch (game_status)
	{
	case GameData::GameStatus::DEFEAT:	// �v���C���[�̔s�k
	{
		if (SceneManager::Instance scene_manager = SceneManager::GetInstance(); scene_manager.Get())
		{
			scene_manager->ChangeScene(new SceneResult);
		}
		break;
	}
	case GameData::GameStatus::VICTORY:	// �v���C���[�̏���
	{
		if (SceneManager::Instance scene_manager = SceneManager::GetInstance(); scene_manager.Get())
		{
			scene_manager->ChangeScene(new SceneLoading(new SceneResult));
		}
		break;
	}
	case GameData::GameStatus::RETURN_TO_TITLE: // �^�C�g���ɖ߂�
	{
		if (SceneManager::Instance scene_manager = SceneManager::GetInstance(); scene_manager.Get())
		{
			scene_manager->ChangeScene(new SceneLoading(new SceneTitle));
		}
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

void SceneGame::PlayerVsEnemy()
{
	// �����蔻��
	if (GameObject::Instance game_object = GameObject::GetInstance(); game_object.Get())
	{
		// �v���C���[�擾
		const auto& player = game_object->GetPlayer();
		if (!player) return;
		if (!player->GetIsActive()) return;	// �v���C���[����A�N�e�B�u�Ȃ珈�����Ȃ�

		// �v���C���[�̍U���p�I�u�W�F�N�g�擾
		const auto& attack_object = player->FindChildObject(MyHash("AttackObject"));

		const auto& player_circle = attack_object->GetComponent<CircleCollisionComponent>();
		if (!player_circle) return;
		if (!player_circle->GetIsActive()) return;

		// �G�̔z��擾
		const auto& enemy_Wptr_pool = game_object->GetEnemyWptPool();

		// �G�擾
		for (const auto& enemy_Wptr : enemy_Wptr_pool)
		{
			auto enemy = enemy_Wptr.lock();
			if (!enemy) continue;
			if (!enemy->GetIsActive()) continue;
			const auto& enemy_circle = enemy->GetComponent<CircleCollisionComponent>();
			if (!enemy_circle) return;
			if (!enemy_circle->GetIsActive()) return;

			// �v���C���[(�U)Vs�G(��)��
			CircleHitResult player_hit_result{};
			CircleHitResult enemy_hit_result{};
			if (Collision::IntersectCircleVsCircle(
				player_circle->GetCircleParam(),
				enemy_circle->GetCircleParam(),
				player_hit_result,
				enemy_hit_result
			))
			{
				// �q�b�g���U���g�ݒ�
				player_circle->SetHitResult(player_hit_result);
				enemy_circle->SetHitResult(enemy_hit_result);

				// �ڐG����
				player_circle->OnCollision(enemy_circle->GetOwner());
			}
		}
	}
}

#ifdef _DEBUG

void SceneGame::DebugDrawGUI()
{
	this->object_manager.DrawDebugGUI();

	// �V�F�[�_�[
	DrawShaderImGui();
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