#include <DirectXCollision.h>

#include "SceneGame.h"
#include "Graphics/Graphics.h"
#include "System/Misc.h"
#include "Camera/CameraManager.h"
#include "Effekseer\EffekseerSystem.h"

// シーン
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

// object定数
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
	// シーン遷移用ステート変数の初期化
	this->change_state = CHANGE_SCENE_STATE::START;

	// ゲーム状態を設定
	{
		GameData::Instance game_data = GameData::GetInstance();
		game_data->SetGameStatus(GameData::GameStatus::GAME);
		game_data->SetScore(0);
		game_data->RestGameTime();
	}

	// シェーダーの作成
	{
		this->instancing_model_shader = std::make_unique<InstancingModelShader>();
		this->model_shader = std::make_unique<ModelShader>();
	}

	// パーティクルシステムのテクスチャロード
	{
		this->particle_system.LoadTexture("Data/Effect/Texture/particle.png");
	}

	// object_managerの作成
	{
		this->object_manager = std::make_unique<ObjectManager>();
	}

	// オブジェクト作成
	{
		// ゲーム時間表示オブジェクト
		UIConstant::CreateGameTimerUI(object_manager->Create("GameTimer"));

		// プレイヤー体力ゲージオブジェクト
		UIConstant::CreatePlayerHpBarUI(object_manager->Create("PlayerHpBar"));

		// プレイヤーのスペシャルゲージ
		UIConstant::CreatePlayerSpecialBar(object_manager->Create("SpecialBar"));

		// 操作説明UI
		UIConstant::CreateDescriptionUI(object_manager->Create("DescriptionUI"));

		// スコアオブジェクトを作成
		const UIConstant::ScoreUIGroup chain_score_ui_group = UIConstant::CreateScoreUIs(object_manager->Create("Total Score"),object_manager->Create("ChainScore Pop UI"), object_manager->Create("ChainScore Move UI"));

		// エネミースポナー作成
		const auto& enemy_spwner = EnemyConstant::CreateEnemySpawner(object_manager->Create("EnemySpawner"), this->object_manager, chain_score_ui_group.chain_pop_ui_object);
#ifdef _DEBUG
		this->enemy_spwner_Wptr = enemy_spwner;
#endif // _DEBUG


		// ステージ(床)
		const auto& stage_floor = StageConstant::CreateStageFloor(object_manager->Create("StageFloor"));

		// ステージ(壁)
		const auto& stage_wall = StageConstant::CreateStageWall(object_manager->Create("StageWall"));

		// プレイヤー
		const auto& player = PlayerConstant::CreatePlayer(object_manager->Create("Player"));

#ifdef _DEBUG	// デバッグ用オブジェクト
		{
			// パーティクル再生用オブジェクト
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

		// GameObjectRegistryに設定
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

	this->grid_object_manager.ClearGridCell();	// グリッドセルの初期化
}

void SceneGame::Finalize()
{
	// オーディオのリセット
	{
		Audio::Instance audio = Audio::GetInstance();
		audio->AllClear();
	}

	// カメラのリセット
	{
		CameraManager::Instance camera_manager = CameraManager::GetInstance();
		camera_manager->Reset();
	}

	// ポーズオブジェクトを非アクティブに設定
	SceneManager::Instance scene_manager = SceneManager::GetInstance();;
	scene_manager->GetPauseObject()->SetIsActive(false);

	EffekseerSystem::GetInstance()->AllStopEffect();
}

void SceneGame::Update(float elapsed_time)
{
	// ポーズ処理
	{
		{
			GameData::Instance game_data = GameData::GetInstance();
			if (game_data->GetIsPause())
			{
				{
					Input::Instance input = Input::GetInstance();
					const auto& game_pad = input->GetGamePad();

					// (X)ゲームに戻る
					if (GamePad::BTN_X & game_pad.GetButtonDown())
					{
						game_data->SetIsPause(false);
						game_data->SetGameStatus(GameData::GameStatus::GAME);

						// ポーズオブジェクトを非アクティブに設定
						SceneManager::Instance scene_manager = SceneManager::GetInstance();;
						scene_manager->GetPauseObject()->SetIsActive(false);
						return;
					}
					// (Y)リトライ
					else if (GamePad::BTN_Y & game_pad.GetButtonDown())
					{
						game_data->SetIsPause(false);
						game_data->SetGameStatus(GameData::GameStatus::RESTART);

						// ポーズオブジェクトを非アクティブに設定
						SceneManager::Instance scene_manager = SceneManager::GetInstance();;
						scene_manager->GetPauseObject()->SetIsActive(false);
						return;
					}
					// (B)タイトルに戻る
					else if (GamePad::BTN_B & game_pad.GetButtonDown())
					{
						game_data->SetIsPause(false);
						game_data->SetGameStatus(GameData::GameStatus::RETURN_TO_TITLE);

						// ポーズオブジェクトを非アクティブに設定
						SceneManager::Instance scene_manager = SceneManager::GetInstance();;
						scene_manager->GetPauseObject()->SetIsActive(false);
						return;
					}
					// (A)ゲーム終了
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

		// パーティクルデモのON・OFF
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
			// タイマー停止
			GameData::Instance game_data = game_data->GetInstance();
			const bool flag = !game_data->GetIsStopTimer();		// タイマー停止、無敵化で共通するフラグ
			game_data->SetIsStopTimer(flag);


			// プレイヤーの無敵化
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
		// 敵生成のON・OFF
		if (GamePad::BTN_DEBUG_TOGGLE_ENEMY_SPAWN & game_pad.GetButtonDown())
		{
			const auto& enemy_spwner = this->enemy_spwner_Wptr.lock();
			if (enemy_spwner)
			{
				enemy_spwner->SetIsActive(!enemy_spwner->GetIsActive());
			}
		}
		// 敵の全削除
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
			// 制限時間を過ぎていたらゲーム状態を変更
			game_data->SetGameStatus(GameData::GameStatus::VICTORY);
		}
	}

	object_manager->Update(elapsed_time);

	{
		GameObjectRegistry::Instance fame_object_registry = GameObjectRegistry::GetInstance();
		// 更新処理
		// 主に削除されたエネミーをリストから消す処理
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

	// ゲーム状態を処理する
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

	// 3Dモデルの描画
	{
		// モデル描画
		this->model_shader->Render();

		// インスタンシング描画
		this->instancing_model_shader->Render();
	}

#ifdef _DEBUG
	// 3Dデバッグ描画
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

	// エフェクシアによるエフェクト描画
	{
		EffekseerSystem::GetInstance()->Render();
	}

	// 2Dスプライト描画
	{
		{
			ParticleSystem::Instance particle_system = ParticleSystem::GetInstance();
			particle_system->Update();
			particle_system->Render();
		}

		// 2Dスプライト描画
		{
			SpriteShader::Instance sprite_shader = SpriteShader::GetInstance();
			sprite_shader->Render();
		}
	}
}

void SceneGame::ReStart()
{
	// シーン遷移用ステート変数の初期化
	this->change_state = CHANGE_SCENE_STATE::START;

	object_manager->ReStart();

	// エネミーの削除
	{
		GameObjectRegistry::Instance game_object = GameObjectRegistry::GetInstance();
		game_object->EnemyAllClear();
	}

	// ゲーム状態をデフォルトに設定
	{
		GameData::Instance game_data = GameData::GetInstance();
		game_data->SetGameStatus(GameData::GameStatus::GAME);
		game_data->SetScore(0);			// スコアリセット
		game_data->RestGameTime();		// 制限時間リセット
	}

	// カメラをリセット
	{
		CameraManager::Instance camera_manager = CameraManager::GetInstance();
		camera_manager->Reset();
	}
}

void SceneGame::ProcessGameState()
{
	GameData::Instance game_data = GameData::GetInstance();

	// ゲーム状態を取得
	const GameData::GameStatus& game_status = game_data->GetGameStatus();

	switch (game_status)
	{
	case GameData::GameStatus::DEFEAT:	// プレイヤーの敗北
	{
		{
			SceneManager::Instance scene_manager = SceneManager::GetInstance();
			switch (this->change_state)
			{
			case CHANGE_SCENE_STATE::START:
			{
				// フェードイン準備
				if (const auto& fade_controller = scene_manager->GetFadeControlle(); fade_controller.get())
				{
					fade_controller->FeadStart(FEAD_TYPE::FEAD_IN, UIConstant::PLAYER_DEFEAT_FEAD_DURATION);
				}

				// デスカメラに遷移
				{
					CameraManager::Instance camera_manager = CameraManager::GetInstance();
					camera_manager->SetCurrentCamera(CAMERA_TYPE::DEATH);
				}

				this->change_state = CHANGE_SCENE_STATE::RUN;
			}
			break;
			case CHANGE_SCENE_STATE::RUN:	// フェードイン待機
			{
				if (const auto& fade_controller = scene_manager->GetFadeControlle(); fade_controller.get())
				{
					// フェード中なら return
					if (fade_controller->GetIsActive()) return;

					// シーン切り替え
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
	case GameData::GameStatus::VICTORY:	// プレイヤーの勝利
	{
		{
			SceneManager::Instance scene_manager = SceneManager::GetInstance();
			switch (this->change_state)
			{
			case CHANGE_SCENE_STATE::START:	// フェードイン準備
			{
				if (const auto& fade_controller = scene_manager->GetFadeControlle(); fade_controller.get())
				{
					fade_controller->FeadStart(FEAD_TYPE::FEAD_IN, UIConstant::DEFAULT_FEAD_DURATION);
				}
				this->change_state = CHANGE_SCENE_STATE::RUN;
			}
				break;
			case CHANGE_SCENE_STATE::RUN:	// フェードイン待機
			{
				if (const auto& fade_controller = scene_manager->GetFadeControlle(); fade_controller.get())
				{
					// フェード中なら return
					if (fade_controller->GetIsActive()) return;

					// シーン切り替え
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
	case GameData::GameStatus::RETURN_TO_TITLE: // タイトルに戻る
	{
		SceneManager::Instance scene_manager = SceneManager::GetInstance();
		scene_manager->ChangeScene(new SceneLoading(new SceneTitle));
		break;
	}
	case GameData::GameStatus::RESTART:	// リスタート
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

	// シェーダー
	DrawShaderImGui();

	// ゲームオブジェクト
	{
		GameObjectRegistry::Instance game_object = GameObjectRegistry::GetInstance();
		game_object->DebugDrawGUI();
	}

	// ライトマネージャー
	{
		LightManager::Instance light_manager = LightManager::GetInstance();
		light_manager->DrawDebugGUI();
	}

	// パーティクルシステム
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