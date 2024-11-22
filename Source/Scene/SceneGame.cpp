#include <imgui.h>
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
#include "Debug/DebugManager.h"
#include "Debug/DebugComponent.h"
#include "Input/Input.h"
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
#ifdef _DEBUG
#include "Component\DebugParticle.h"
#endif // DEBUG


#pragma region LegacyComponent(旧コンポーネント)
#include "Component/LegacyStateMachineComponent.h"
#pragma endregion LegacyComponent(旧コンポーネント)

#include "StateMachine/LegacyTransitionJudgementDerived.h"
#include "StateMachine/LegacyStateDerived.h"
#include "StateMachine/StateDerived.h"


void SceneGame::Initialize()
{
	// シェーダーの作成
	{
		instancing_model_shader = std::make_unique<InstancingModelShader>();
		model_shader = std::make_unique<ModelShader>();
		sprite_shader = std::make_unique<SpriteShader>();
	}

	// パーティクルシステムのテクスチャロード
	{
		particle_system.LoadTexture("Data/Effect/Texture/t0006.png");
	}

	// オブジェクト作成
	{
		// ステージ
		{
			auto stage = object_manager.Create("Stage");
			stage->AddComponent<ModelComponent>("Data/Debug/Model/Cube/Cube.mdl");
			// トランスフォーム設定
			{
				Transform3DComponent::Transform3DParam param{};
				param.local_scale = DirectX::XMFLOAT3(600.0f, 1.0f, 600.0f);
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

		// プレイヤー
		{
			auto player = object_manager.Create("Player");

			// コリジョンに設定するコンポーネントは事前に作成しておく
			std::shared_ptr<PlayerComponent> player_component;// プレイヤーコンポーネント

			// モデル設定
			{
				auto model = player->AddComponent<ModelComponent>("Data/Model/Player/Player.mdl");
			}
			// アニメーション設定
			{
				auto model_animation = player->AddComponent<ModelAnimationControlComponent>("Data/Model/Player/Player.mdl");
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
				auto shader_component =
					player->AddComponent<ModelShaderComponent>(model_shader.get());
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
					auto debug_Particle = player->CreateChildObject();
					debug_Particle->SetName("Debug Particle");
					// transform
					{
						Transform3DComponent::Transform3DParam param{};
						param.local_position = DirectX::XMFLOAT3(0.0f, 50.0f, 0.0f);
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
				auto debug_Particle = object_manager.Create("Debug Particle");

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
		}
#endif // _DEBUG

		// 敵
		{
			float territory_range = 220.0f;
#ifdef _DEBUG
			for (int i = 0; i < 500; ++i)
#else
			for (int i = 0; i < 5000; ++i)
#endif // _DEBUG
			{
				auto enemy = object_manager.Create();

				// コリジョンに設定するコンポーネントは事前に作成しておく
				std::shared_ptr<EnemyComponent> enemy_component;

				// エネミーコンポーネント設定
				{
					EnemyComponent::EnemyParam param{};
					enemy_component = enemy->AddComponent<EnemyComponent>(param);
				}
				// ムーブメント設定
				{
					auto movement = enemy->AddComponent<MovementComponent>(MovementComponent::MovementParam());
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
					float range = MyMathf::RandomRange(0.0f, territory_range);

					Transform3DComponent::Transform3DParam param{};
					param.local_position =
					{
#if 0	// 初期値固定
						0.0f,
						0.0f,
						0.0f,
#else
						sinf(theta) * range,
						0.5f,
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
		}
		
		#ifdef _DEBUG
				if (Audio::Instance audio = Audio::GetInstance(); audio.Get())
				{
					AudioParam param{};
					param.volume = 0.3f;
					param.loop = true;
					param.filename = "Data/Audio/BGM.wav";
					audio->Play(param);
				}
		#endif // _DEBUG
	}
}

void SceneGame::Finalize()
{
	// オーディオのリセット
	if (Audio::Instance audio = Audio::GetInstance(); audio.Get())
	{
		audio->AllClear();
	}
}

void SceneGame::Update(float elapsed_time)
{
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
	// スペースキーでSE再生
	if (Input::Instance input = Input::GetInstance(); input.Get())
	{
		GamePad& game_pad = input->GetGamePad();
		if (GamePad::BTN_Y & game_pad.GetButtonDown())
		{
			//if (Audio::Instance audio = Audio::GetInstance(); audio.Get())
			//{
			//	AudioParam param{};
			//	param.filename = "Data/Debug/Audio/SE.wav";
			//	audio->Play(param);
			//}
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
		sprite_shader->Render();
	}
}

void SceneGame::ReStart()
{
	object_manager.ReStart();

	// ゲーム状態をデフォルトに設定
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
		if (ImGui::CollapsingHeader(sprite_shader->GetName()))
		{
			sprite_shader->DrawDebugGUI();
		}
	}
	ImGui::End();
}

#endif // _DEBUG