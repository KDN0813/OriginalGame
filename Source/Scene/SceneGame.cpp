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

			if (GameObject* game_object = GameObject::Instance())
			{
				game_object->SetStage(stage);
			}
		}

		// �v���C���[
		{
			auto player = object_manager.Create("Player");
			auto model = player->AddComponent<ModelComponent>("Data/Model/Player/Player.mdl");
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
			// �L�����N�^�[�X�e�[�^�X
			{
				CharacterComponent::CharacterParam param{};
				param.max_hp = 100;
				param.hp = 50;
				player->AddComponent<CharacterComponent>(param);
			}

			// �v���C���[�̍U������p�I�u�W�F�N�g
			{
				std::shared_ptr<Object> player_attack_object = player->CreateChildObject();
				player_attack_object->SetName("AttackObject");
				// �g�����X�t�H�[���ݒ�
				{
					Transform3DComponent::Transform3DParam param{};
					param.local_position = DirectX::XMFLOAT3(0.0f, 0.0f, 100.0f);
					param.local_scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

					auto transform = player_attack_object->AddComponent<Transform3DComponent>(param);
				}
				// ���[�u�����g�ݒ�
				{
					auto movement = player_attack_object->AddComponent<MovementComponent>(MovementComponent::MovementParam());
				}
				// �~�̃R���C�_�[
				{
					CircleCollisionComponent::CollisionParam param{};
					param.collision_type = COLLISION_TYPE::ATTACKER;
					param.default_active_flag = false;
					auto collision = player_attack_object->AddComponent<CircleCollisionComponent>(param);
				}
			}

			// GameObject�ɐݒ�
			if (GameObject* game_object = GameObject::Instance())
			{
				game_object->SetPlayer(player);
			}
		}


		// �f�X�J����
		{
			auto death_camera = object_manager.Create("Death Camera");
			CameraManager* camera_manager = CameraManager::Instance();
			if (camera_manager)
			{
				auto camera = death_camera->AddComponent<CameraComponent>(camera_manager->GetCamera(CAMERA_TYPE::DEATH));
			}
		}

		// �G
		{
			float territory_range = 45.0f;
			for (int i = 0; i < 1; ++i)
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
						// �ړ�
						model->SetAnimationState(EnemyCT::ANIMATION::MOVE_FWD, true);
						model->AddAnimationTransition(EnemyCT::ANIMATION::MOVE_FWD, EnemyCT::ANIMATION::IDLE_BATTLE, std::make_unique<Judgement_IsAtTarget>(enemy));
						model->AddAnimationTransition(EnemyCT::ANIMATION::MOVE_FWD, EnemyCT::ANIMATION::TAUNTING, std::make_unique<Judgement_HitDamage>(enemy));
						// �U��
						model->SetAnimationState(EnemyCT::ANIMATION::IDLE_BATTLE, true);
						model->AddAnimationTransition(EnemyCT::ANIMATION::IDLE_BATTLE, EnemyCT::ANIMATION::MOVE_FWD, std::make_unique<Judgement_IdleFinished>(enemy));
						model->AddAnimationTransition(EnemyCT::ANIMATION::IDLE_BATTLE, EnemyCT::ANIMATION::TAUNTING, std::make_unique<Judgement_HitDamage>(enemy));
						// �_���[�W(��)
						model->SetAnimationState(EnemyCT::ANIMATION::TAUNTING, false);
						model->AddAnimationTransition(EnemyCT::ANIMATION::TAUNTING, EnemyCT::ANIMATION::MOVE_FWD, std::make_unique<Judgement_TransitionReady>(enemy));
					}
				}
				// �X�e�[�g�ݒ�
				auto state_machine = enemy->AddComponent<LegacyStateMachineComponent>();
				{
					auto idle_state = state_machine->RegisterState<Legacy_IdelState>();
					idle_state->AddStateTransition(std::make_unique<LegacyStateTransitionInfo>("WanderState", std::make_unique<Judgement_IdleFinished>(enemy)), LegacyStateBase::JudgementUpdatePhase::PostUpdate);
					idle_state->AddStateTransition(std::make_unique<LegacyStateTransitionInfo>("DamageState", std::make_unique<Judgement_HitDamage>(enemy)), LegacyStateBase::JudgementUpdatePhase::PostUpdate);
					auto wander_state = state_machine->RegisterState<Legacy_WanderState>();
					wander_state->AddStateTransition(std::make_unique<LegacyStateTransitionInfo>("IdelState", std::make_unique<Judgement_IsAtTarget>(enemy)), LegacyStateBase::JudgementUpdatePhase::PostUpdate);
					wander_state->AddStateTransition(std::make_unique<LegacyStateTransitionInfo>("DamageState", std::make_unique<Judgement_HitDamage>(enemy)), LegacyStateBase::JudgementUpdatePhase::PostUpdate);
					auto damage_state = state_machine->RegisterState<Legacy_DamageState>();
					damage_state->AddStateTransition(std::make_unique<LegacyStateTransitionInfo>("WanderState", std::make_unique<Judgement_TransitionReady>(enemy)), LegacyStateBase::JudgementUpdatePhase::PostUpdate);
					auto death_idle_state = state_machine->RegisterState<Legacy_DeathIdleState>();

					state_machine->SetDefaultState("WanderState");
				}
				// �~�̃R���C�_�[
				{
					CircleCollisionComponent::CollisionParam param{};
					param.collision_type = COLLISION_TYPE::DEFENDER;
					auto collision = enemy->AddComponent<CircleCollisionComponent>(param);
				}
				// �g�����X�t�H�[���ݒ�
				{
					float offset = 2.0f;
					float theta = MyMathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
					float range = MyMathf::RandomRange(0.0f, territory_range);

					Transform3DComponent::Transform3DParam param{};
					param.local_position  = 
					{
						//sinf(theta) * range,
						0.0f,
						0.0f,
						0.0f,
						//cosf(theta) * range ,
					};
					param.local_scale = DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f);

					auto transform = enemy->AddComponent<Transform3DComponent>(param);
				}

				// �L�����N�^�[�X�e�[�^�X
				{
					CharacterComponent::CharacterParam param{};
					param.max_hp = 10;
					param.hp = 5;
					enemy->AddComponent<CharacterComponent>(param);
				}

				// �V�F�[�_�[�ݒ�
				auto shader_component =
					enemy->AddComponent<InstancingModelShaderComponent>(this->instancing_model_shader.get());
			
				if (GameObject* game_object = GameObject::Instance())
				{
					game_object->SetEnemy(enemy);
				}
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

	Audio::Instance()->Update();

	PlayerVsEnemy();

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

	CameraManager* camera_manager = CameraManager::Instance();
	if (camera_manager)
	{
		camera_manager->Reset();
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

void SceneGame::PlayerVsEnemy()
{
	// �����蔻��
	if (GameObject* game_object = GameObject::Instance())
	{
		// �v���C���[�擾
		const auto& player = game_object->GetPlayer();
		if (!player) return;
		if (!player->GetIsActive()) return;	// �v���C���[����A�N�e�B�u�Ȃ珈�����Ȃ�

		// �v���C���[�̍U���p�I�u�W�F�N�g�擾
		const auto& attack_object = player->FindChildObject(MyHash("AttackObject"));

		const auto& player_circle = attack_object->GetComponent<CircleCollisionComponent>();
		if (!player_circle) return;

		// �q�b�g�t���O�̍X�V
		player_circle->SetHitOldFlag(player_circle->GetHitFlag());
		player_circle->SetHitFlag(false);
		if (!player_circle->GetIsActive()) return;

		// �G�擾
		for (const std::shared_ptr<Object>& enemy : game_object->GetEnemyPool())
		{
			if (!enemy) continue;
			if (!enemy->GetIsActive()) continue;
			const auto& enemy_circle = enemy->GetComponent<CircleCollisionComponent>();
			if (!enemy_circle) return;
			// �q�b�g�t���O�̍X�V
			enemy_circle->SetHitOldFlag(enemy_circle->GetHitFlag());
			enemy_circle->SetHitFlag(false);
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
				// ������(�ŏI�I�ɍ폜����)
				{
					// �Փ˂����ꍇ�A�U�����Ɩh�䑤�Ƀq�b�g�t���O��ݒ�
					player_circle->SetHitFlag(true);
					enemy_circle->SetHitFlag(true);

					// �U�����̃��U���g�ݒ�
					player_hit_result.hit_object_Wptr = enemy_circle->GetOwner();
					player_circle->SetHitResult(player_hit_result);

					// �h�䑤�̃��U���g�ݒ�
					enemy_hit_result.hit_object_Wptr = player_circle->GetOwner();
					enemy_circle->SetHitResult(enemy_hit_result);
				}

				// �ڐG����
				player_circle->OnCollision(player_circle->GetOwner());
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