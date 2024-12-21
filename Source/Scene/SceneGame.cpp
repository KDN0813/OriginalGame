#include <DirectXCollision.h>

#include "SceneGame.h"
#include "Graphics/Graphics.h"
#include "System/Misc.h"
#include "Camera/CameraManager.h"

// �V�[��
#include "Scene/SceneManager.h"
#include "Scene/SceneTitle.h"
#include "Scene/SceneResult.h"
#include "Scene/SceneLoading.h"

#include "Shader/SpriteShader.h"

#ifdef _DEBUG
#include <imgui.h>
#include "Debug/DebugManager.h"
#include "Debug/DebugComponent.h"
#endif // _DEBUG

#include "Input/Input.h"
#include "Input/GamePad.h"
#include "System/GameData.h"
#include "ConstantManager.h"

// object�萔
#include "Object\Constant\PlayerConstant.h"
#include "Object\Constant\EnemyConstant.h"
#include "Object\Constant\StageConstant.h"
#include "Object\Constant\UIConstant.h"

#include "System/MyMath/MyMathf.h"
#include "Collision/Collision.h"
#include "Audio/Audio.h"

#include "Component\TransformComponent.h"
#include "Component\InstancedModelWithAnimationComponent.h"
#include "Component\CircleCollisionComponent.h"
#include "Component\CameraComponent.h"
#ifdef _DEBUG
#include "Component\ModelShaderComponent.h"
#include "Component\DebugParticle.h"
#endif // DEBUG


#pragma region LegacyComponent(���R���|�[�l���g)
#include "Component/LegacyStateMachineComponent.h"
#pragma endregion LegacyComponent(���R���|�[�l���g)

#include "StateMachine/LegacyTransitionJudgementDerived.h"
#include "StateMachine/LegacyStateDerived.h"
#include "StateMachine/PlayerStateDerived.h"
#include "StateMachine/UIStateDerived.h"


void SceneGame::Initialize()
{
	// �Q�[����Ԃ�ݒ�
	if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
	{
		game_data->SetGameStatus(GameData::GameStatus::GAME);
		game_data->SetScore(0);
		game_data->RestGameTime();
	}

	// �V�F�[�_�[�̍쐬
	{
		instancing_model_shader = std::make_unique<InstancingModelShader>();
		model_shader = std::make_unique<ModelShader>();
		sky_box = std::make_unique<SkyBox>();
	}

	// �p�[�e�B�N���V�X�e���̃e�N�X�`�����[�h
	{
		particle_system.LoadTexture("Data/Effect/Texture/t0006.png");
	}

	// �I�u�W�F�N�g�쐬
	{
		// �X�R�A�\���p�I�u�W�F�N�g
		UIConstant::CreateScoreUI(object_manager.Create("Score"));

		// �Q�[�����ԕ\���I�u�W�F�N�g
		UIConstant::CreateGameTimerUI(object_manager.Create("GameTimer"));

		// �X�e�[�W(��)
		const auto& stage_foor = StageConstant::CreateStageFloor(object_manager.Create("StageFloor"));

		// �X�e�[�W(��)
		const auto& stage_wall = StageConstant::CreateStageWall(object_manager.Create("StageWall"));

		// �v���C���[
		const auto& player = PlayerConstant::CreatePlayer(object_manager.Create("Player"));

		// �f�X�J����
		{
			auto death_camera = object_manager.Create("Death Camera");
			if (CameraManager::Instance camera_manager = CameraManager::GetInstance(); camera_manager.Get())
			{
				death_camera->AddComponent<CameraComponent>(camera_manager->GetCamera(CAMERA_TYPE::DEATH));
			}
		}

#ifdef _DEBUG	// �f�o�b�O�p�I�u�W�F�N�g
		{
			// �p�[�e�B�N���Đ��p�I�u�W�F�N�g
			{
				this->debug_Particle = object_manager.Create("Debug Particle");

				// transform
				{
					Transform3DComponent::Transform3DParam param{};
					param.local_position = DirectX::XMFLOAT3(0.0f, 0.5f, -20.0f);
					this->debug_Particle->AddComponent<Transform3DComponent>(param);
				}
				// DebugParticle
				{
					this->debug_Particle->AddComponent<DebugParticle>();
				}
			}
		}
#endif // _DEBUG

		// �G
		{
			if (GameObject::Instance game_object = GameObject::GetInstance(); game_object.Get())
			{
#ifdef _DEBUG
				for (int i = 0; i < 100; ++i)
#else
				for (int i = 0; i < this->enemy_max; ++i)
#endif // _DEBUG
				{
					const auto& enemy = EnemyConstant::CreateEnemy(object_manager.Create(("enemy" + std::to_string(i)).c_str()));

					game_object->SetEnemy(enemy);
				}

				// �Q�[���I�u�W�F�N�g�ݒ�
				game_object->SetPlayer(player);
				game_object->SetStageFoor(stage_foor);
				game_object->SetStageWall(stage_wall);
			}
		}

		if (Audio::Instance audio = Audio::GetInstance(); audio.Get())
		{
			AudioParam param{};
			param.volume = 0.0f;
			param.loop = true;
			param.filename = "Data/Audio/BGM.wav";
			audio->Play(param);
		}
	}
}

void SceneGame::Finalize()
{
	// �I�[�f�B�I�̃��Z�b�g
	if (Audio::Instance audio = Audio::GetInstance(); audio.Get())
	{
		audio->AllClear();
	}

	// �J�����̃��Z�b�g
	if(CameraManager::Instance camera_manager = CameraManager::GetInstance(); camera_manager.Get())
	{
		camera_manager->Reset();
	}

	// �|�[�Y����
	if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
	{
		//this->pause_object->SetIsActive(false);
	}
}

void SceneGame::Update(float elapsed_time)
{
	// �|�[�Y����
	{
		if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
		{
			if (game_data->GetIsPause())
			{
				if (Input::Instance input = Input::GetInstance(); input.Get())
				{
					const auto& game_pad = input->GetGamePad();

					// (X)�Q�[���ɖ߂�
					if (GamePad::BTN_X & game_pad.GetButtonDown())
					{
						game_data->SetIsPause(false);
						game_data->SetGameStatus(GameData::GameStatus::GAME);
						return;
					}
					// (Y)���g���C
					else if (GamePad::BTN_Y & game_pad.GetButtonDown())
					{
						game_data->SetIsPause(false);
						game_data->SetGameStatus(GameData::GameStatus::RESTART);
						return;
					}
					// (B)�^�C�g���ɖ߂�
					else if (GamePad::BTN_B & game_pad.GetButtonDown())
					{
						game_data->SetIsPause(false);
						game_data->SetGameStatus(GameData::GameStatus::RETURN_TO_TITLE);
						return;
					}

				}
				return;
			}
		}
	}

#ifdef _DEBUG
	if (Input::Instance input = Input::GetInstance(); input.Get())
	{
		const auto& game_pad = input->GetGamePad();

		// �f����Ԃ̐ݒ�E����
		if (GamePad::BTN_DEBUG_DEMO & game_pad.GetButtonDown())
		{
			if (const auto& particle = this->debug_Particle->GetComponent<DebugParticle>())
			{
				particle->ReverseEffectLooping();
			}
		}
	}

#endif // _DEBUG


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


	if(GameObject::Instance game_object = GameObject::GetInstance() ; game_object.Get())
	{
		// �X�V����
		// ��ɍ폜���ꂽ�G�l�~�[�����X�g�����������
		game_object->Update();

		// �J�����O�̓G�I�u�W�F�N�g���A�N�e�B�u�ɂ���
		// �J�����͈͂ƃL�����̃{�b�N�X�Ŕ�����Ƃ�
		//if (CameraManager::Instance camera_manager = CameraManager::GetInstance(); camera_manager.Get())
		//{
		//	for (const auto& enemy_Wprt : game_object->GetEnemyWptPool())
		//	{
		//		if (const auto& enemy = enemy_Wprt.lock())
		//		{
		//			if (const auto& instancing_model = enemy->GetComponent<InstancedModelWithAnimationComponent>())
		//			{
		//				enemy->SetIsActive(camera_manager->IsAnyMeshAABBVisible(instancing_model->GetBoundingBoxs()));
		//			}
		//		}
		//	}
		//}

		// �v���C���[�̈ړ��͈͐���
		// TODO �X�e�[�W��ύX������X�e�[�W�ɍ��킹���ړ��͈͂̐�����������
		{
			const auto& player = game_object->GetPlayer();
			if (const auto& player_transform = player->GetComponent<Transform3DComponent>())
			{
				DirectX::XMFLOAT3 player_pos = player_transform->GetWorldPosition();

				// TODO ���z�u(�}�W�b�N�i���o�[)����߂�
				float territory_range = 185.0f;
				if (territory_range < player_pos.x)
				{
					player_transform->AddLocalPosition({ territory_range - player_pos.x,0.0f,0.0f });
				}
				if (player_pos.x < -territory_range)
				{
					player_transform->AddLocalPosition({ -territory_range - player_pos.x,0.0f,0.0f });
				}
				if (territory_range < player_pos.z)
				{
					player_transform->AddLocalPosition({ 0.0f,0.0f,territory_range - player_pos.z });
				}
				if (player_pos.z < -territory_range)
				{
					player_transform->AddLocalPosition({ 0.0f,0.0f,-territory_range - player_pos.z });
				}
			}
		}
	}

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
	if (Input::Instance input = Input::GetInstance(); input.Get())
	{
		GamePad& game_pad = input->GetGamePad();
		if (GamePad::BTN_DEBUG_GAME_RESET & game_pad.GetButtonDown())
		{
			ReStart();
		}
	}
#endif // DEBUG

	// �Q�[����Ԃ���������
	ProcessGameState();

	if (ParticleSystem::Instance particle_system = ParticleSystem::GetInstance(); particle_system.Get())
	{
		particle_system->SetElapsedTime(elapsed_time);
	}
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
	if (GameData::Instance game_data = GameData::GetInstance(); (game_data.Get() && game_data->GetDrawDebugPrimitiveFlag()))
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
		if (ParticleSystem::Instance particle_system = ParticleSystem::GetInstance(); particle_system.Get())
		{
			particle_system->Update();
			particle_system->Render();
		}

		// 2D�X�v���C�g�`��
		if (SpriteShader::Instance sprite_shader = SpriteShader::GetInstance(); sprite_shader.Get())
		{
			sprite_shader->Render();
		}
	}
}

void SceneGame::ReStart()
{
	object_manager.ReStart();

	// �G�l�~�[�̐���ǉ�����
	if (GameObject::Instance game_object = GameObject::GetInstance(); game_object.Get())
	{
		const size_t now_enemy_count = game_object->GetEnemyWptPool().size();
		for (int i = 0; i < this->enemy_max - now_enemy_count; ++i)
		{
			const auto& enemy = EnemyConstant::CreateEnemy(object_manager.Create());
			game_object->SetEnemy(enemy);
		}
	}

	// �Q�[����Ԃ��f�t�H���g�ɐݒ�
	if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
	{
		game_data->SetGameStatus(GameData::GameStatus::GAME);
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

	// ���C�g�}�l�[�W���[
	if (LightManager::Instance light_manager = LightManager::GetInstance(); light_manager.Get())
	{
		light_manager->DrawDebugGUI();
	}

	// �p�[�e�B�N���V�X�e��
	if (ParticleSystem::Instance particle_system = ParticleSystem::GetInstance(); particle_system.Get())
	{
		particle_system->DebugDrawGUI();
	}
}

void SceneGame::DrawShaderImGui()
{
	if (ImGui::Begin("Shader"))
	{
		if (ImGui::CollapsingHeader(model_shader->GetName()))
		{
			model_shader->DrawDebugGUI();
		}
		if (ImGui::CollapsingHeader(instancing_model_shader->GetName()))
		{
			instancing_model_shader->DrawDebugGUI();
		}
		if (SpriteShader::Instance sprite_shader = SpriteShader::GetInstance(); sprite_shader.Get())
		{
			if (ImGui::CollapsingHeader(sprite_shader->GetName()))
			{
				sprite_shader->DrawDebugGUI();
			}
		}
	}
	ImGui::End();
}

#endif // _DEBUG