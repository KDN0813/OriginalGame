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
#include "ConstantManager.h"

// object定数
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


#pragma region LegacyComponent(旧コンポーネント)
#include "Component/LegacyStateMachineComponent.h"
#pragma endregion LegacyComponent(旧コンポーネント)

#include "StateMachine/LegacyTransitionJudgementDerived.h"
#include "StateMachine/LegacyStateDerived.h"
#include "StateMachine/PlayerStateDerived.h"
#include "StateMachine/UIStateDerived.h"


void SceneGame::Initialize()
{
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
		particle_system.LoadTexture("Data/Effect/Texture/t0006.png");
	}

	// オブジェクト作成
	{
		// スコア表示用オブジェクト
		UIConstant::CreateScoreUI(object_manager.Create("Score"));

		// ゲーム時間表示オブジェクト
		UIConstant::CreateGameTimerUI(object_manager.Create("GameTimer"));

		// ステージ(床)
		const auto& stage_foor = StageConstant::CreateStageFloor(object_manager.Create("StageFloor"));

		// ステージ(壁)
		const auto& stage_wall = StageConstant::CreateStageWall(object_manager.Create("StageWall"));

		// プレイヤー
		const auto& player = PlayerConstant::CreatePlayer(object_manager.Create("Player"));

		// デスカメラ
		{
			auto death_camera = object_manager.Create("Death Camera");
			if (CameraManager::Instance camera_manager = CameraManager::GetInstance(); camera_manager.Get())
			{
				death_camera->AddComponent<CameraComponent>(camera_manager->GetCamera(CAMERA_TYPE::DEATH));
			}
		}

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
#ifdef _DEBUG
				for (int i = 0; i < 100; ++i)
#else
				for (int i = 0; i < this->enemy_max; ++i)
#endif // _DEBUG
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

	// ポーズ解除
	if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
	{
		//this->pause_object->SetIsActive(false);
	}
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
						return;
					}
					// (Y)リトライ
					else if (GamePad::BTN_Y & game_pad.GetButtonDown())
					{
						game_data->SetIsPause(false);
						game_data->SetGameStatus(GameData::GameStatus::RESTART);
						return;
					}
					// (B)タイトルに戻る
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

		// カメラ外の敵オブジェクトを非アクティブにする
		// カメラ範囲とキャラのボックスで判定をとる
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

		// プレイヤーの移動範囲制限
		// TODO ステージを変更したらステージに合わせた移動範囲の制限をかける
		{
			const auto& player = game_object->GetPlayer();
			if (const auto& player_transform = player->GetComponent<Transform3DComponent>())
			{
				DirectX::XMFLOAT3 player_pos = player_transform->GetWorldPosition();

				// TODO 仮配置(マジックナンバー)をやめる
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
			scene_manager->ChangeScene(new SceneResult);
		}
		break;
	}
	case GameData::GameStatus::VICTORY:	// プレイヤーの勝利
	{
		if (SceneManager::Instance scene_manager = SceneManager::GetInstance(); scene_manager.Get())
		{
			scene_manager->ChangeScene(new SceneLoading(new SceneResult));
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

void SceneGame::PlayerVsEnemy()
{
	// 当たり判定
	if (GameObject::Instance game_object = GameObject::GetInstance(); game_object.Get())
	{
		// プレイヤー取得
		const auto& player = game_object->GetPlayer();
		if (!player) return;
		if (!player->GetIsActive()) return;	// プレイヤーが非アクティブなら処理しない

		// プレイヤーの攻撃用オブジェクト取得
		const auto& attack_object = player->FindChildObject(MyHash("AttackObject"));

		const auto& player_circle = attack_object->GetComponent<CircleCollisionComponent>();
		if (!player_circle) return;
		if (!player_circle->GetIsActive()) return;

		// 敵の配列取得
		const auto& enemy_Wptr_pool = game_object->GetEnemyWptPool();

		// 敵取得
		for (const auto& enemy_Wptr : enemy_Wptr_pool)
		{
			auto enemy = enemy_Wptr.lock();
			if (!enemy) continue;
			if (!enemy->GetIsActive()) continue;
			const auto& enemy_circle = enemy->GetComponent<CircleCollisionComponent>();
			if (!enemy_circle) return;
			if (!enemy_circle->GetIsActive()) return;

			// プレイヤー(攻)Vs敵(受)の
			CircleHitResult player_hit_result{};
			CircleHitResult enemy_hit_result{};
			if (Collision::IntersectCircleVsCircle(
				player_circle->GetCircleParam(),
				enemy_circle->GetCircleParam(),
				player_hit_result,
				enemy_hit_result
			))
			{
				// ヒットリザルト設定
				player_circle->SetHitResult(player_hit_result);
				enemy_circle->SetHitResult(enemy_hit_result);

				// 接触処理
				player_circle->OnCollision(enemy_circle->GetOwner());
			}
		}
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