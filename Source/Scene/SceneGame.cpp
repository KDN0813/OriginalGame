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

#ifdef _DEBUG
#include <imgui.h>
#include "Debug/DebugManager.h"
#include "Debug/DebugComponent.h"
#endif // _DEBUG

#include "Input/Input.h"
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
#include "Component\SpriteComponent.h"
#ifdef _DEBUG
#include "Component\DebugParticle.h"
#endif // DEBUG


#pragma region LegacyComponent(旧コンポーネント)
#include "Component/LegacyStateMachineComponent.h"
#pragma endregion LegacyComponent(旧コンポーネント)

#include "StateMachine/LegacyTransitionJudgementDerived.h"
#include "StateMachine/LegacyStateDerived.h"
#include "StateMachine/PlayerStateDerived.h"


void SceneGame::Initialize()
{
	// ゲーム状態を設定
	if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
	{
		game_data->SetGameStatus(GameData::GameStatus::GAME);
	}

	// シェーダーの作成
	{
		instancing_model_shader = std::make_unique<InstancingModelShader>();
		model_shader = std::make_unique<ModelShader>();
		sprite_shader = std::make_unique<SpriteShader>();
		sky_box = std::make_unique<SkyBox>();
	}

	// パーティクルシステムのテクスチャロード
	{
		particle_system.LoadTexture("Data/Effect/Texture/t0006.png");
	}

	// オブジェクト作成
	{
		// ポーズ画面用オブジェクト作成
		{
			this->pause_object = object_manager.Create("pause_object");

			// スプライト読み込み
			{
				SpriteComponent::SpriteParam param{};
				param.color = { 1.0f,1.0f, 1.0f, 1.0f };
				param.filename = "Data/Sprite/Pause.png";
				auto sprite = pause_object->AddComponent<SpriteComponent>(param);
				this->sprite_shader->AddSprite(sprite);
			}
		}

		// ステージ
		{
			auto stage = object_manager.Create("Stage");
			auto model = stage->AddComponent<ModelComponent>("Data/Model/Stage/testStage.mdl");
			//model->SetTileCount(150.0f);
			// トランスフォーム設定
			{
				Transform3DComponent::Transform3DParam param{};
				param.local_scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
				param.local_position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
				auto transform = stage->AddComponent<Transform3DComponent>(param);
			}
			// シェーダー設定
			auto shader_component =
				stage->AddComponent<ModelShaderComponent>(model_shader.get());

			if (GameObject::Instance game_object = GameObject::GetInstance(); game_object.Get())
			{
				game_object->SetStage(stage);
			}
		}

		// スカイボックス
		{
			auto sky_box = object_manager.Create("Sky Box");

			// テクスチャ
			//{
			//	SpriteComponent::SpriteParam param{};
			//	param.filename = "";

			//	auto sprite = sky_box->AddComponent<SpriteComponent>(param);
			//}
		}

		// プレイヤー
		{
			auto player = object_manager.Create("Player");

			// コリジョンに設定するコンポーネントは事前に作成しておく
			std::shared_ptr<PlayerComponent> player_component;// プレイヤーコンポーネント

			// モデル設定
			{
				auto model = player->AddComponent<ModelComponent>("Data/Debug/Model/Player/Player.mdl");
			}
			// アニメーション設定
			{
				ModelAnimationControlComponent::InitAnimeParam param{};
				param.init_anime_index = PlayerCT::IDLE;
				param.init_anime_loop = true;
				auto model_animation = player->AddComponent<ModelAnimationControlComponent>(param);
			}
			// ステートマシン設定
			auto state_machine = player->AddComponent<StateMachineComponent>();
			{
				// ステートの追加
				state_machine->RegisterState<PlayerIdleState>();
				state_machine->RegisterState<PlayerMoveState>();
				state_machine->RegisterState<PlayerAttackState>();
				state_machine->RegisterState<PlayerSpinAttackState>();

				state_machine->SetDefaultState("PlayerIdleState");	// デフォルトステートの設定
			}

			// トランスフォーム設定
			{
				Transform3DComponent::Transform3DParam param{};
				param.local_scale = DirectX::XMFLOAT3(0.018f, 0.018f, 0.018f);
				param.local_position = DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f);
				auto transform = player->AddComponent<Transform3DComponent>(param);
			}
			// ムーブメント設定
			{
				MovementComponent::MovementParam param{};
				param.max_accelerationXZ = 7.0f;
				param.is_stage_raycas = true;
				auto movement = player->AddComponent<MovementComponent>(param);
			}
			// プレイヤーコンポーネント作成
			{
				player_component = player->AddComponent<PlayerComponent>(PlayerComponent::PlayerParam());
			}
			// シェーダー設定
			{
				auto shader_component = player->AddComponent<ModelShaderComponent>(model_shader.get());
			}
			// 重力
			{
				player->AddComponent<GravityComponent>(GravityComponent::GravityParam());
			}
			// キャラクターステータス
			{
				CharacterComponent::CharacterParam param{};
				param.max_hp = 100;
				param.hp = 50;
				player->AddComponent<CharacterComponent>(param);
			}


			// 子オブジェクト
			{
				// プレイヤーカメラ
				{
					std::shared_ptr<Object> player_camera_object = player->CreateChildObject();
					player_camera_object->SetName("PlayerCamera");
					// カメラ設定
					{
						if (CameraManager::Instance camera_manager = CameraManager::GetInstance(); camera_manager.Get())
						{
							auto camera = player_camera_object->AddComponent<CameraComponent>(camera_manager->GetCamera(CAMERA_TYPE::MAIN));
						}
					}
					// カメラコントローラー設定
					{
						CameraControllerGamepad::CameraControllerParam param{};
						player_camera_object->AddComponent<CameraControllerGamepad>(param);
					}
					// トランスフォーム設定
					{
						Transform3DComponent::Transform3DParam param{};
						player_camera_object->AddComponent<Transform3DComponent>(param);
					}
				}
				// プレイヤーの攻撃判定用オブジェクト
				{
					std::shared_ptr<Object> player_attack_object = player->CreateChildObject();
					player_attack_object->SetName("AttackObject");
					// トランスフォーム設定
					{
						Transform3DComponent::Transform3DParam param{};
						param.local_position = DirectX::XMFLOAT3(0.0f, 0.0f, 100.0f);
						param.local_scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

						auto child_transform = player_attack_object->AddComponent<Transform3DComponent>(param);
					}
					// ムーブメント設定
					{
						auto child_movement = player_attack_object->AddComponent<MovementComponent>(MovementComponent::MovementParam());
					}
					// 円のコライダー
					{
						CircleCollisionComponent::CollisionParam param{};
						param.collision_type = COLLISION_OBJECT_TYPE::PLAYER;
						param.radius = 2.0f;
						param.default_active_flag = false;
						auto child_collision = player_attack_object->AddComponent<CircleCollisionComponent>(param);

						// 接触時処理するコンポーネントの追加
						{
							child_collision->AddCollisionEntercomponent(player_component);
						}
					}
				}
				// 回転攻撃用当たり判定オブジェクト
				{
					std::shared_ptr<Object> player_attack_object = player->CreateChildObject();
					player_attack_object->SetName("SpinAttackObject");
					// トランスフォーム設定
					{
						Transform3DComponent::Transform3DParam param{};
						param.local_position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
						param.local_scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

						auto child_transform = player_attack_object->AddComponent<Transform3DComponent>(param);
					}
					// 円のコライダー
					{
						CircleCollisionComponent::CollisionParam param{};
						param.collision_type = COLLISION_OBJECT_TYPE::PLAYER;
						param.radius = 5.0f;
						param.default_active_flag = false;
						auto child_collision = player_attack_object->AddComponent<CircleCollisionComponent>(param);

						// 接触時処理するコンポーネントの追加
						{
							child_collision->AddCollisionEntercomponent(player_component);
						}
					}
				}
#ifdef _DEBUG
				// パーティクル再生用オブジェクト
				{
					const auto& debug_Particle = player->CreateChildObject();
					debug_Particle->SetName("Debug Particle");
					// transform
					{
						Transform3DComponent::Transform3DParam param{};
						param.local_position = DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f);
						debug_Particle->AddComponent<Transform3DComponent>(param);
					}
					// DebugParticle
					{
						debug_Particle->AddComponent<DebugParticle>();
					}
				}
#endif // DEBUG
			}

			// GameObjectに設定
			if (GameObject::Instance game_object = GameObject::GetInstance(); game_object.Get())
			{
				game_object->SetPlayer(player);
			}
		}


		// デスカメラ
		{
			auto death_camera = object_manager.Create("Death Camera");
			if (CameraManager::Instance camera_manager = CameraManager::GetInstance(); camera_manager.Get())
			{
				auto camera = death_camera->AddComponent<CameraComponent>(camera_manager->GetCamera(CAMERA_TYPE::DEATH));
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
#ifdef _DEBUG
			for (int i = 0; i < 0; ++i)
#else
			for (int i = 0; i < this->enemy_max; ++i)
#endif // _DEBUG
			{
				CreateEnemy(object_manager.Create());
			}
		}

		// オブジェクトの開始処理を実行
		object_manager.Start();

		if (Audio::Instance audio = Audio::GetInstance(); audio.Get())
		{
			AudioParam param{};
			param.volume = 0.4f;
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
		//game_data->UpdateGameEndTimer(elapsed_time);
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
		if (CameraManager::Instance camera_manager = CameraManager::GetInstance(); camera_manager.Get())
		{
			for (const auto& enemy_Wprt : game_object->GetEnemyWptPool())
			{
				if (const auto& enemy = enemy_Wprt.lock())
				{
					if (const auto& instancing_model = enemy->GetComponent<InstancedModelWithAnimationComponent>())
					{
						enemy->SetIsActive(camera_manager->IsAnyMeshAABBVisible(instancing_model->GetBoundingBoxs()));
					}
				}
			}
		}

		// プレイヤーの移動範囲制限
		// TODO ステージを変更したらステージに合わせた移動範囲の制限をかける
		{
			const auto& player = game_object->GetPlayer();
			if (const auto& player_transform = player->GetComponent<Transform3DComponent>())
			{
				DirectX::XMFLOAT3 player_pos = player_transform->GetWorldPosition();

				// TODO 仮配置(マジックナンバー)をやめる
				float territory_range = 220.0f;
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
		// ポーズ処理
		{
			if (GameData::Instance game_data = GameData::GetInstance(); game_data.Get())
			{
				this->pause_object->SetIsActive(game_data->GetIsPause());
			}
		}

		if (ParticleSystem::Instance particle_system = ParticleSystem::GetInstance(); particle_system.Get())
		{
			particle_system->Update();
			particle_system->Render();
		}
		sprite_shader->Render();
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
			CreateEnemy(object_manager.Create());
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

void SceneGame::CreateEnemy(const std::shared_ptr<Object>& enemy)
{
	// TODO 仮配置(マジックナンバー)をやめる
	float territory_range = EnemyComponent::DEFAULT_TERRITORY_RENGR;
	float player_area_rage = 10.0f;

	// コリジョンに設定するコンポーネントは事前に作成しておく
	std::shared_ptr<EnemyComponent> enemy_component;

	// エネミーコンポーネント設定
	{
		EnemyComponent::EnemyParam param{};
		enemy_component = enemy->AddComponent<EnemyComponent>(param);
	}
	// ムーブメント設定
	{
		MovementComponent::MovementParam param{};
		param.turn_speed = 100.0f;
		auto movement = enemy->AddComponent<MovementComponent>(param);
	}
	// モデル設定
	{
		InstancedModelWithAnimationComponent::InstancedModelParam param;
		param.anime_index = EnemyCT::ANIMATION::IDLE_BATTLE;
		param.anime_loop = true;
		param.anime_play = true;

		auto model = enemy->AddComponent<InstancedModelWithAnimationComponent>(param, "Data/Model/ChestMonster/ChestMonster.mdl");
	}
	// トランスフォーム設定
	{
		float offset = 2.0f;
		float theta = MyMathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
		float range = MyMathf::RandomRange(player_area_rage, territory_range);

		Transform3DComponent::Transform3DParam param{};
		param.local_position =
		{
#if 0	// 初期値固定
						0.0f,
						0.0f,
						0.0f,
#else
						sinf(theta) * range,
						1.0f,
						cosf(theta) * range ,
#endif
		};
		param.local_scale = DirectX::XMFLOAT3(0.015f, 0.015f, 0.015f);

		auto transform = enemy->AddComponent<Transform3DComponent>(param);
	}

	// キャラクターステータス
	{
		CharacterComponent::CharacterParam param{};
		param.max_hp = 10;
		param.hp = 2;
		enemy->AddComponent<CharacterComponent>(param);
	}

	// シェーダー設定
	auto shader_component =
		enemy->AddComponent<InstancingModelShaderComponent>(this->instancing_model_shader.get());

	// 円のコライダー
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

	// 子オブジェクト
	{
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
		if (ImGui::CollapsingHeader(sprite_shader->GetName()))
		{
			sprite_shader->DrawDebugGUI();
		}
	}
	ImGui::End();
}

#endif // _DEBUG