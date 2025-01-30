#include <DirectXCollision.h>

#include "SceneGame.h"
#include "Graphics/Graphics.h"
#include "System/Misc.h"
#include "Camera/CameraManager.h"

// シーン
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
#endif // DEBUG


#pragma region LegacyComponent(旧コンポーネント)
#include "Component/LegacyStateMachineComponent.h"
#pragma endregion LegacyComponent(旧コンポーネント)

#include "StateMachine/LegacyTransitionJudgementDerived.h"
#include "StateMachine/LegacyStateDerived.h"
#include "StateMachine/PlayerStateDerived.h"
#include "StateMachine/UIStateDerived.h"


void SceneGame::Initialize()
{
	// シーン遷移用ステート変数の初期化
	this->change_state = CHANGE_SCENE_STATE::START;

	// ゲーム状態を設定
	if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
	{
		game_data->SetGameStatus(GameData::GameStatus::GAME);
		game_data->SetScore(0);
		game_data->RestGameTime();
	}

	// シェーダーの作成
	{
		instancing_model_shader = std::make_unique<InstancingModelShader>();
		model_shader = std::make_unique<ModelShader>();
		sky_box = std::make_unique<SkyBox>();
	}

	// パーティクルシステムのテクスチャロード
	{
		particle_system.LoadTexture("Data/Effect/Texture/particle.png");
	}

	// オブジェクト作成
	{
		// スコア表示用オブジェクト
		UIConstant::CreateScoreUI(object_manager.Create("Score"));

		// ゲーム時間表示オブジェクト
		UIConstant::CreateGameTimerUI(object_manager.Create("GameTimer"));

		// プレイヤー体力ゲージオブジェクト
		UIConstant::CreatePlayerHpBarUI(object_manager.Create("PlayerHpBar"));

		// ステージ(床)
		const auto& stage_foor = StageConstant::CreateStageFloor(object_manager.Create("StageFloor"));

		// ステージ(壁)
		const auto& stage_wall = StageConstant::CreateStageWall(object_manager.Create("StageWall"));

		// プレイヤー
		const auto& player = PlayerConstant::CreatePlayer(object_manager.Create("Player"));

#ifdef _DEBUG	// デバッグ用オブジェクト
		{
			// パーティクル再生用オブジェクト
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

		// 敵
		{
			if (GameObject::Instance game_object = GameObject::GetInstance(); game_object.Get())
			{
				for (int i = 0; i < this->enemy_max; ++i)
				{
					const auto& enemy = EnemyConstant::CreateEnemy(object_manager.Create(("enemy" + std::to_string(i)).c_str()));

					game_object->SetEnemy(enemy);
				}

				// ゲームオブジェクト設定
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

	this->grid_object_manager.ClearGridCell();	// グリッドセルの初期化
}

void SceneGame::Finalize()
{
	// オーディオのリセット
	if (Audio::Instance audio = Audio::GetInstance(); audio.Get())
	{
		audio->AllClear();
	}

	// カメラのリセット
	if(CameraManager::Instance camera_manager = CameraManager::GetInstance(); camera_manager.Get())
	{
		camera_manager->Reset();
	}

	// ポーズオブジェクトを非アクティブに設定
	SceneManager::Instance scene_manager = SceneManager::GetInstance();;
	scene_manager->GetPauseObject()->SetIsActive(false);
}

void SceneGame::Update(float elapsed_time)
{
	// ポーズ処理
	{
		if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
		{
			if (game_data->GetIsPause())
			{
				if (Input::Instance input = Input::GetInstance(); input.Get())
				{
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
	if (Input::Instance input = Input::GetInstance(); input.Get())
	{
		const auto& game_pad = input->GetGamePad();

		// デモ状態の設定・解除
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
			// 制限時間を過ぎていたらゲーム状態を変更
			game_data->SetGameStatus(GameData::GameStatus::VICTORY);
		}
	}

	object_manager.Update(elapsed_time);


	if(GameObject::Instance game_object = GameObject::GetInstance() ; game_object.Get())
	{
		// 更新処理
		// 主に削除されたエネミーをリストから消す処理
		game_object->Update();
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
		collision_manager->VsPlayer();
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

	// ゲーム状態を処理する
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

	// 3Dモデルの描画
	{
		// モデル描画
		this->model_shader->Render();

		// インスタンシング描画
		this->instancing_model_shader->Render();
	}

#ifdef _DEBUG
	// 3Dデバッグ描画
	if (GameData::Instance game_data = GameData::GetInstance(); (game_data.Get() && game_data->GetDrawDebugPrimitiveFlag()))
	{
		object_manager.DrawDebugPrimitive();
		if (DebugManager::Instance debug_manager = DebugManager::GetInstance(); debug_manager.Get())
		{
			debug_manager->GetDebugPrimitiveRenderer()->Render();
		}
	}

	grid_object_manager.DrawDebugPrimitive();;
#endif // _DEBUG

	// 2Dスプライト描画
	{
		if (ParticleSystem::Instance particle_system = ParticleSystem::GetInstance(); particle_system.Get())
		{
			particle_system->Update();
			particle_system->Render();
		}

		// 2Dスプライト描画
		if (SpriteShader::Instance sprite_shader = SpriteShader::GetInstance(); sprite_shader.Get())
		{
			sprite_shader->Render();
		}
	}
}

void SceneGame::ReStart()
{
	// シーン遷移用ステート変数の初期化
	this->change_state = CHANGE_SCENE_STATE::START;

	object_manager.ReStart();

	// エネミーの数を追加する
	if (GameObject::Instance game_object = GameObject::GetInstance(); game_object.Get())
	{
		const size_t now_enemy_count = game_object->GetEnemyWptPool().size();
		for (int i = 0; i < this->enemy_max - now_enemy_count; ++i)
		{
			const auto& enemy = EnemyConstant::CreateEnemy(object_manager.Create());
			game_object->SetEnemy(enemy);
		}
	}

	// ゲーム状態をデフォルトに設定
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

	// ゲーム状態を取得
	const GameData::GameStatus& game_status = game_data->GetGameStatus();

	switch (game_status)
	{
	case GameData::GameStatus::DEFEAT:	// プレイヤーの敗北
	{
		if (SceneManager::Instance scene_manager = SceneManager::GetInstance(); scene_manager.Get())
		{
			switch (this->change_state)
			{
			case CHANGE_SCENE_STATE::START:
			{
				// フェードイン準備
				if (const auto& fead_controlle = scene_manager->GetFadeControlle(); fead_controlle.get())
				{
					fead_controlle->SetFead(FEAD_TYPE::FEAD_OUT, UIConstant::PLAYER_DEFEAT_FEAD_DURATION);
					fead_controlle->FeadStart();
				}

				// デスカメラに遷移
				if (CameraManager::Instance camera_manager = CameraManager::GetInstance(); camera_manager.Get())
				{
					camera_manager->SetCurrentCamera(CAMERA_TYPE::DEATH);
				}

				this->change_state = CHANGE_SCENE_STATE::RUN;
			}
			break;
			case CHANGE_SCENE_STATE::RUN:	// フェードイン待機
			{
				if (const auto& fead_controlle = scene_manager->GetFadeControlle(); fead_controlle.get())
				{
					// フェード中ならreturn
					if (fead_controlle->GetIsActive()) return;

					// シーン切り替え
					scene_manager->ChangeScene(new SceneLoading(new SceneTitle));
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
		if (SceneManager::Instance scene_manager = SceneManager::GetInstance(); scene_manager.Get())
		{
			switch (this->change_state)
			{
			case CHANGE_SCENE_STATE::START:	// フェードイン準備
			{
				if (const auto& fead_controlle = scene_manager->GetFadeControlle(); fead_controlle.get())
				{
					fead_controlle->SetFead(FEAD_TYPE::FEAD_OUT, UIConstant::DEFAULT_FEAD_DURATION);
					fead_controlle->FeadStart();
				}
				this->change_state = CHANGE_SCENE_STATE::RUN;
			}
				break;
			case CHANGE_SCENE_STATE::RUN:	// フェードイン待機
			{
				if (const auto& fead_controlle = scene_manager->GetFadeControlle(); fead_controlle.get())
				{
					// フェード中ならreturn
					if (fead_controlle->GetIsActive()) return;

					// シーン切り替え
					scene_manager->ChangeScene(new SceneLoading(new SceneTitle));
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
		if (SceneManager::Instance scene_manager = SceneManager::GetInstance(); scene_manager.Get())
		{
			scene_manager->ChangeScene(new SceneLoading(new SceneTitle));
		}
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
	this->object_manager.DrawDebugGUI();

	// シェーダー
	DrawShaderImGui();

	// ライトマネージャー
	if (LightManager::Instance light_manager = LightManager::GetInstance(); light_manager.Get())
	{
		light_manager->DrawDebugGUI();
	}

	// パーティクルシステム
	if (ParticleSystem::Instance particle_system = ParticleSystem::GetInstance(); particle_system.Get())
	{
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