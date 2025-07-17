#include <DirectXCollision.h>

#include "SceneGame.h"
#include "Graphics/Graphics.h"
#include "System/Misc.h"
#include "Camera/CameraManager.h"
#include "Effekseer\EffekseerSystem.h"

// �V�[��
#include "Scene/SceneManager.h"
#include "Scene/SceneTitle.h"
#include "Scene/SceneResult.h"
#include "Scene/SceneLoading.h"

#include "Shader/SpriteShader.h"

#ifdef _DEBUG
#include <imgui.h>
#include "Debug/DebugManager.h"
#endif // _DEBUG

#include "Input/Input.h"
#include "Input/GamePad.h"
#include "System/GameData.h"

// object�萔
#include "Object\Constant\PlayerConstant.h"
#include "Object\Constant\EnemyConstant.h"
#include "Object\Constant\StageConstant.h"
#include "Object\Constant\UIConstant.h"
#include "Object\Constant\CameraConstant.h"

#include "System/MyMath/MyMathf.h"
#include "Collision/Collision.h"
#include "Audio/Audio.h"

#include "Component\TransformComponent.h"
#include "Component\InstancedModelWithAnimationComponent.h"
#include "Component\CircleCollisionComponent.h"
#include "Component\CameraComponent.h"
#include "Component\FadeControllerComponent.h"
#ifdef _DEBUG
#include "Component\ModelShaderComponent.h"
#include "Component\DebugParticle.h"
#include "Component\CharacterComponent.h"
#endif // DEBUG

#include "StateMachine/PlayerStateDerived.h"
#include "StateMachine/UIStateDerived.h"

void SceneGame::Initialize()
{
	// �V�[���J�ڗp�X�e�[�g�ϐ��̏�����
	this->change_state = CHANGE_SCENE_STATE::START;

	// �Q�[����Ԃ�ݒ�
	{
		GameData::Instance game_data = GameData::GetInstance();
		game_data->SetGameStatus(GameData::GameStatus::GAME);
		game_data->SetScore(0);
		game_data->RestGameTime();
	}

	// �V�F�[�_�[�̍쐬
	{
		this->instancing_model_shader = std::make_unique<InstancingModelShader>();
		this->model_shader = std::make_unique<ModelShader>();
	}

	// �p�[�e�B�N���V�X�e���̃e�N�X�`�����[�h
	{
		this->particle_system.LoadTexture("Data/Effect/Texture/particle.png");
	}

	// object_manager�̍쐬
	{
		this->object_manager = std::make_unique<ObjectManager>();
	}

	// �I�u�W�F�N�g�쐬
	{
		// �Q�[�����ԕ\���I�u�W�F�N�g
		UIConstant::CreateGameTimerUI(object_manager->Create("GameTimer"));

		// �v���C���[�̗̓Q�[�W�I�u�W�F�N�g
		UIConstant::CreatePlayerHpBarUI(object_manager->Create("PlayerHpBar"));

		// �v���C���[�̃X�y�V�����Q�[�W
		UIConstant::CreatePlayerSpecialBar(object_manager->Create("SpecialBar"));

		// �������UI
		UIConstant::CreateDescriptionUI(object_manager->Create("DescriptionUI"));

		// �X�R�A�I�u�W�F�N�g���쐬
		const UIConstant::ScoreUIGroup chain_score_ui_group = UIConstant::CreateScoreUIs(object_manager->Create("Total Score"),object_manager->Create("ChainScore Pop UI"), object_manager->Create("ChainScore Move UI"));

		// �G�l�~�[�X�|�i�[�쐬
		const auto& enemy_spwner = EnemyConstant::CreateEnemySpawner(object_manager->Create("EnemySpawner"), this->object_manager, chain_score_ui_group.chain_pop_ui_object);
#ifdef _DEBUG
		this->enemy_spwner_Wptr = enemy_spwner;
#endif // _DEBUG


		// �X�e�[�W(��)
		const auto& stage_floor = StageConstant::CreateStageFloor(object_manager->Create("StageFloor"));

		// �X�e�[�W(��)
		const auto& stage_wall = StageConstant::CreateStageWall(object_manager->Create("StageWall"));

		// �v���C���[
		const auto& player = PlayerConstant::CreatePlayer(object_manager->Create("Player"));

#ifdef _DEBUG	// �f�o�b�O�p�I�u�W�F�N�g
		{
			// �p�[�e�B�N���Đ��p�I�u�W�F�N�g
			{
				this->debug_Particle = object_manager->Create("Debug Particle");
				this->debug_Particle->SetIsActive(false);

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

		// GameObjectRegistry�ɐݒ�
		{
			GameObjectRegistry::Instance game_object = GameObjectRegistry::GetInstance();
			game_object->SetPlayer(player);
			game_object->SetStageFloor(stage_floor);
			game_object->SetStageWall(stage_wall);
		}

		if (Audio::Instance audio = Audio::GetInstance(); audio.Get())
		{
			AudioParam param{};
#ifdef _DEBUG
			param.volume = 0.0f;
#else
			param.volume = 0.5f;
#endif // _DEBUG

			param.loop = true;
			param.filename = "Data/Audio/BGM.wav";
			audio->Play(param);
		}
	}

	this->grid_object_manager.ClearGridCell();	// �O���b�h�Z���̏�����
}

void SceneGame::Finalize()
{
	// �I�[�f�B�I�̃��Z�b�g
	{
		Audio::Instance audio = Audio::GetInstance();
		audio->AllClear();
	}

	// �J�����̃��Z�b�g
	{
		CameraManager::Instance camera_manager = CameraManager::GetInstance();
		camera_manager->Reset();
	}

	// �|�[�Y�I�u�W�F�N�g���A�N�e�B�u�ɐݒ�
	SceneManager::Instance scene_manager = SceneManager::GetInstance();;
	scene_manager->GetPauseObject()->SetIsActive(false);

	EffekseerSystem::GetInstance()->AllStopEffect();
}

void SceneGame::Update(float elapsed_time)
{
	// �|�[�Y����
	{
		{
			GameData::Instance game_data = GameData::GetInstance();
			if (game_data->GetIsPause())
			{
				{
					Input::Instance input = Input::GetInstance();
					const auto& game_pad = input->GetGamePad();

					// (X)�Q�[���ɖ߂�
					if (GamePad::BTN_X & game_pad.GetButtonDown())
					{
						game_data->SetIsPause(false);
						game_data->SetGameStatus(GameData::GameStatus::GAME);

						// �|�[�Y�I�u�W�F�N�g���A�N�e�B�u�ɐݒ�
						SceneManager::Instance scene_manager = SceneManager::GetInstance();;
						scene_manager->GetPauseObject()->SetIsActive(false);
						return;
					}
					// (Y)���g���C
					else if (GamePad::BTN_Y & game_pad.GetButtonDown())
					{
						game_data->SetIsPause(false);
						game_data->SetGameStatus(GameData::GameStatus::RESTART);

						// �|�[�Y�I�u�W�F�N�g���A�N�e�B�u�ɐݒ�
						SceneManager::Instance scene_manager = SceneManager::GetInstance();;
						scene_manager->GetPauseObject()->SetIsActive(false);
						return;
					}
					// (B)�^�C�g���ɖ߂�
					else if (GamePad::BTN_B & game_pad.GetButtonDown())
					{
						game_data->SetIsPause(false);
						game_data->SetGameStatus(GameData::GameStatus::RETURN_TO_TITLE);

						// �|�[�Y�I�u�W�F�N�g���A�N�e�B�u�ɐݒ�
						SceneManager::Instance scene_manager = SceneManager::GetInstance();;
						scene_manager->GetPauseObject()->SetIsActive(false);
						return;
					}
					// (A)�Q�[���I��
					else if (GamePad::BTN_A & game_pad.GetButtonDown())
					{
						game_data->CloseWindow();
						return;
					}
				}
				return;
			}
		}
	}

#ifdef _DEBUG
	{
		Input::Instance input = Input::GetInstance();
		const auto& game_pad = input->GetGamePad();

		// �p�[�e�B�N���f����ON�EOFF
		if (GamePad::BTN_DEBUG_DEMO & game_pad.GetButtonDown())
		{
			if (const auto& particle = this->debug_Particle->GetComponent<DebugParticle>())
			{
				particle->ReverseEffectLooping();
				debug_Particle->SetIsActive(particle->GetEffectLooping());
			}
		}
		if (GamePad::BTN_DEBUG_PLAYER_INVINCIBLE & game_pad.GetButtonDown())
		{
			GameObjectRegistry::Instance fame_object_registry = GameObjectRegistry::GetInstance();
			const auto& player = fame_object_registry->GetPlayer();

			std::shared_ptr<CharacterComponent> player_character;
			if (player)
			{
				if (std::shared_ptr<CharacterComponent> player_character = player->GetComponent<CharacterComponent>())
				{
					player_character->SetInvincibleFlag(!player_character->GetInvincibleFlag());
				}
			}
		}
		if (GamePad::BTN_DEBUG_TOGGLE_INVINCIBLE_AND_TIMER & game_pad.GetButtonDown())
		{
			// �^�C�}�[��~
			GameData::Instance game_data = game_data->GetInstance();
			const bool flag = !game_data->GetIsStopTimer();		// �^�C�}�[��~�A���G���ŋ��ʂ���t���O
			game_data->SetIsStopTimer(flag);


			// �v���C���[�̖��G��
			GameObjectRegistry::Instance fame_object_registry = GameObjectRegistry::GetInstance();
			const auto& player = fame_object_registry->GetPlayer();

			std::shared_ptr<CharacterComponent> player_character;
			if (player)
			{
				if (std::shared_ptr<CharacterComponent> player_character = player->GetComponent<CharacterComponent>())
				{
					player_character->SetInvincibleFlag(flag);
				}
			}
		}
		// �G������ON�EOFF
		if (GamePad::BTN_DEBUG_TOGGLE_ENEMY_SPAWN & game_pad.GetButtonDown())
		{
			const auto& enemy_spwner = this->enemy_spwner_Wptr.lock();
			if (enemy_spwner)
			{
				enemy_spwner->SetIsActive(!enemy_spwner->GetIsActive());
			}
		}
		// �G�̑S�폜
		if (GamePad::BTN_DEBUG_REMOVE_ALL_ENEMIES & game_pad.GetButtonDown())
		{
			GameObjectRegistry::Instance object_registry = GameObjectRegistry::GetInstance();
			const auto& enemyWpt_pool = object_registry->GetEnemyWptPool();

			for (const auto& enemy_Wptr : enemyWpt_pool)
			{
				if (const auto& enemy = enemy_Wptr.lock())
				{
					enemy->SetIsRemove(true);
				}
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

	object_manager->Update(elapsed_time);

	{
		GameObjectRegistry::Instance fame_object_registry = GameObjectRegistry::GetInstance();
		// �X�V����
		// ��ɍ폜���ꂽ�G�l�~�[�����X�g�����������
		fame_object_registry->Update();
	}

	{
		CameraManager::Instance camera_manager = CameraManager::GetInstance();
		camera_manager->Update(elapsed_time);
	}

	{
		Audio::Instance audio = Audio::GetInstance();
		audio->Update();
	}

	{
		CircleCollisionManager::Instance collision_manager = CircleCollisionManager::GetInstance();
		collision_manager->VsEnemy();
		collision_manager->VsPlayer();
	}

#ifdef _DEBUG
	{
		Input::Instance input = Input::GetInstance();
		GamePad& game_pad = input->GetGamePad();
		if (GamePad::BTN_DEBUG_GAME_RESET & game_pad.GetButtonDown())
		{
			
			GameData::GetInstance()->SetIsStopTimer(false);
			ReStart();
		}
	}
#endif // DEBUG

	// �Q�[����Ԃ���������
	ProcessGameState();

	{
		ParticleSystem::Instance particle_system = ParticleSystem::GetInstance();
		particle_system->SetElapsedTime(elapsed_time);
	}

	EffekseerSystem::GetInstance()->Update(elapsed_time);
}

void SceneGame::Render()
{
	{
		Graphics::Instance graphics = Graphics::GetInstance();
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
	if (GameData::Instance game_data = GameData::GetInstance(); (game_data->GetDrawDebugPrimitiveFlag()))
	{
		object_manager->DrawDebugPrimitive();
		{
			DebugManager::Instance debug_manager = DebugManager::GetInstance();
			debug_manager->GetDebugPrimitiveRenderer()->Render();
		}
	}

	grid_object_manager.DrawDebugPrimitive();;
#endif // _DEBUG

	// �G�t�F�N�V�A�ɂ��G�t�F�N�g�`��
	{
		EffekseerSystem::GetInstance()->Render();
	}

	// 2D�X�v���C�g�`��
	{
		{
			ParticleSystem::Instance particle_system = ParticleSystem::GetInstance();
			particle_system->Update();
			particle_system->Render();
		}

		// 2D�X�v���C�g�`��
		{
			SpriteShader::Instance sprite_shader = SpriteShader::GetInstance();
			sprite_shader->Render();
		}
	}
}

void SceneGame::ReStart()
{
	// �V�[���J�ڗp�X�e�[�g�ϐ��̏�����
	this->change_state = CHANGE_SCENE_STATE::START;

	object_manager->ReStart();

	// �G�l�~�[�̍폜
	{
		GameObjectRegistry::Instance game_object = GameObjectRegistry::GetInstance();
		game_object->EnemyAllClear();
	}

	// �Q�[����Ԃ��f�t�H���g�ɐݒ�
	{
		GameData::Instance game_data = GameData::GetInstance();
		game_data->SetGameStatus(GameData::GameStatus::GAME);
		game_data->SetScore(0);			// �X�R�A���Z�b�g
		game_data->RestGameTime();		// �������ԃ��Z�b�g
	}

	// �J���������Z�b�g
	{
		CameraManager::Instance camera_manager = CameraManager::GetInstance();
		camera_manager->Reset();
	}
}

void SceneGame::ProcessGameState()
{
	GameData::Instance game_data = GameData::GetInstance();

	// �Q�[����Ԃ��擾
	const GameData::GameStatus& game_status = game_data->GetGameStatus();

	switch (game_status)
	{
	case GameData::GameStatus::DEFEAT:	// �v���C���[�̔s�k
	{
		{
			SceneManager::Instance scene_manager = SceneManager::GetInstance();
			switch (this->change_state)
			{
			case CHANGE_SCENE_STATE::START:
			{
				// �t�F�[�h�C������
				if (const auto& fade_controller = scene_manager->GetFadeControlle(); fade_controller.get())
				{
					fade_controller->FeadStart(FEAD_TYPE::FEAD_IN, UIConstant::PLAYER_DEFEAT_FEAD_DURATION);
				}

				// �f�X�J�����ɑJ��
				{
					CameraManager::Instance camera_manager = CameraManager::GetInstance();
					camera_manager->SetCurrentCamera(CAMERA_TYPE::DEATH);
				}

				this->change_state = CHANGE_SCENE_STATE::RUN;
			}
			break;
			case CHANGE_SCENE_STATE::RUN:	// �t�F�[�h�C���ҋ@
			{
				if (const auto& fade_controller = scene_manager->GetFadeControlle(); fade_controller.get())
				{
					// �t�F�[�h���Ȃ� return
					if (fade_controller->GetIsActive()) return;

					// �V�[���؂�ւ�
					scene_manager->ChangeScene(new SceneResult);
					this->change_state = CHANGE_SCENE_STATE::END;
				}
			}
			break;
			default:break;
			}
		}
		break;
	}
	case GameData::GameStatus::VICTORY:	// �v���C���[�̏���
	{
		{
			SceneManager::Instance scene_manager = SceneManager::GetInstance();
			switch (this->change_state)
			{
			case CHANGE_SCENE_STATE::START:	// �t�F�[�h�C������
			{
				if (const auto& fade_controller = scene_manager->GetFadeControlle(); fade_controller.get())
				{
					fade_controller->FeadStart(FEAD_TYPE::FEAD_IN, UIConstant::DEFAULT_FEAD_DURATION);
				}
				this->change_state = CHANGE_SCENE_STATE::RUN;
			}
				break;
			case CHANGE_SCENE_STATE::RUN:	// �t�F�[�h�C���ҋ@
			{
				if (const auto& fade_controller = scene_manager->GetFadeControlle(); fade_controller.get())
				{
					// �t�F�[�h���Ȃ� return
					if (fade_controller->GetIsActive()) return;

					// �V�[���؂�ւ�
					scene_manager->ChangeScene(new SceneResult);
					this->change_state = CHANGE_SCENE_STATE::END;
				}
			}
				break;
			default:break;
			}
		}
		break;
	}
	case GameData::GameStatus::RETURN_TO_TITLE: // �^�C�g���ɖ߂�
	{
		SceneManager::Instance scene_manager = SceneManager::GetInstance();
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
	this->object_manager->DrawDebugGUI();

	// �V�F�[�_�[
	DrawShaderImGui();

	// �Q�[���I�u�W�F�N�g
	{
		GameObjectRegistry::Instance game_object = GameObjectRegistry::GetInstance();
		game_object->DebugDrawGUI();
	}

	// ���C�g�}�l�[�W���[
	{
		LightManager::Instance light_manager = LightManager::GetInstance();
		light_manager->DrawDebugGUI();
	}

	// �p�[�e�B�N���V�X�e��
	{
		ParticleSystem::Instance particle_system = ParticleSystem::GetInstance();
		particle_system->DebugDrawGUI();
	}

	// grid_object_manager
	grid_object_manager.DrawDebugGUI();
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