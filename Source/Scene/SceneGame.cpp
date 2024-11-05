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

#pragma region LegacyComponent(旧コンポーネント)
#include "Component/LegacyStateMachineComponent.h"
#pragma endregion LegacyComponent(旧コンポーネント)

#include "StateMachine/LegacyTransitionJudgementDerived.h"
#include "StateMachine/LegacyStateDerived.h"
#include "StateMachine/StateDerived.h"


void SceneGame::Initialize()
{
	Graphics* graphics = Graphics::Instance();
	// シェーダーの作成
	{
		instancing_model_shader = std::make_unique<InstancingModelShader>();
		model_shader = std::make_unique<ModelShader>();
		sprite_shader = std::make_unique<SpriteShader>();
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
				param.local_scale = DirectX::XMFLOAT3(100.0f, 1.0f, 100.0f);
				param.local_position = DirectX::XMFLOAT3(0.0f, -0.5f, 0.0f);
				auto transform = stage->AddComponent<Transform3DComponent>(param);
			}
			// シェーダー設定
			auto shader_component =
				stage->AddComponent<ModelShaderComponent>(model_shader.get());

			if (GameObject* game_object = GameObject::Instance())
			{
				game_object->SetStage(stage);
			}
		}

		// プレイヤー
		{
			auto player = object_manager.Create("Player");
			auto model = player->AddComponent<ModelComponent>("Data/Model/Player/Player.mdl");
			//auto model = player->AddComponent<AnimatedInstancedModelComponent>(device, "Data/Model/Player/Player.mdl");
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

				state_machine->SetDefaultState("PlayerIdleState");	// デフォルトステートの設定
			}

			// トランスフォーム設定
			{
				Transform3DComponent::Transform3DParam param{};
				param.local_scale = DirectX::XMFLOAT3(0.008f, 0.008f, 0.008f);
				auto transform = player->AddComponent<Transform3DComponent>(param);
			}
			// ムーブメント設定
			{
				MovementComponent::MovementParam param{};
				param.is_stage_raycas = true;
				auto movement = player->AddComponent<MovementComponent>(param);
			}
			{
				player->AddComponent<PlayerComponent>(PlayerComponent::PlayerParam());
			}
			// シェーダー設定
			auto shader_component =
				player->AddComponent<ModelShaderComponent>(model_shader.get());
			//auto shader_component =
			//player->AddComponent<InstancingModelShaderComponent>(this->instancing_model_shader.get());
			// カメラ設定
			{
				CameraManager* camera_manager = CameraManager::Instance();
				if (camera_manager)
				{
					auto camera = player->AddComponent<CameraComponent>(camera_manager->GetCamera(CAMERA_TYPE::MAIN));
				}

			}
			// カメラコントローラー設定
			{
				CameraControllerGamepad::CameraControllerParam param{};
				player->AddComponent<CameraControllerGamepad>(param);
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

			// プレイヤーの攻撃判定用オブジェクト
			{
				std::shared_ptr<Object> player_attack_object = player->CreateChildObject();
				player_attack_object->SetName("AttackObject");
				// トランスフォーム設定
				{
					Transform3DComponent::Transform3DParam param{};
					param.local_position = DirectX::XMFLOAT3(0.0f, 0.0f, 100.0f);
					param.local_scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

					auto transform = player_attack_object->AddComponent<Transform3DComponent>(param);
				}
				// ムーブメント設定
				{
					auto movement = player_attack_object->AddComponent<MovementComponent>(MovementComponent::MovementParam());
				}
				// 円のコライダー
				{
					CircleCollisionComponent::CollisionParam param{};
					param.collision_type = COLLISION_TYPE::ATTACKER;
					param.default_active_flag = false;
					auto collision = player_attack_object->AddComponent<CircleCollisionComponent>(param);
				}
			}

			// GameObjectに設定
			if (GameObject* game_object = GameObject::Instance())
			{
				game_object->SetPlayer(player);
			}
		}


		// デスカメラ
		{
			auto death_camera = object_manager.Create("Death Camera");
			CameraManager* camera_manager = CameraManager::Instance();
			if (camera_manager)
			{
				auto camera = death_camera->AddComponent<CameraComponent>(camera_manager->GetCamera(CAMERA_TYPE::DEATH));
			}
		}

		// 敵
		{
			float territory_range = 45.0f;
			for (int i = 0; i < 1; ++i)
			{
				auto enemy = object_manager.Create();
				// エネミーコンポーネント設定
				{
					EnemyComponent::EnemyParam param{};
					auto enemy_component = enemy->AddComponent<EnemyComponent>(param);
				}
				// ムーブメント設定
				{
					auto movement = enemy->AddComponent<MovementComponent>(MovementComponent::MovementParam());
				}
				// アニメーション設定
				{
					AnimatedInstancedModelComponent::InstancedModelParam param;
					param.anime_index = EnemyCT::ANIMATION::IDLE_BATTLE;
					param.anime_loop = true;
					param.anime_play = true;

					auto model = enemy->AddComponent<AnimatedInstancedModelComponent>(param, "Data/Model/ChestMonster/ChestMonster.mdl");
					{
						// 移動
						model->SetAnimationState(EnemyCT::ANIMATION::MOVE_FWD, true);
						model->AddAnimationTransition(EnemyCT::ANIMATION::MOVE_FWD, EnemyCT::ANIMATION::IDLE_BATTLE, std::make_unique<Judgement_IsAtTarget>(enemy));
						model->AddAnimationTransition(EnemyCT::ANIMATION::MOVE_FWD, EnemyCT::ANIMATION::TAUNTING, std::make_unique<Judgement_HitDamage>(enemy));
						// 攻撃
						model->SetAnimationState(EnemyCT::ANIMATION::IDLE_BATTLE, true);
						model->AddAnimationTransition(EnemyCT::ANIMATION::IDLE_BATTLE, EnemyCT::ANIMATION::MOVE_FWD, std::make_unique<Judgement_IdleFinished>(enemy));
						model->AddAnimationTransition(EnemyCT::ANIMATION::IDLE_BATTLE, EnemyCT::ANIMATION::TAUNTING, std::make_unique<Judgement_HitDamage>(enemy));
						// ダメージ(仮)
						model->SetAnimationState(EnemyCT::ANIMATION::TAUNTING, false);
						model->AddAnimationTransition(EnemyCT::ANIMATION::TAUNTING, EnemyCT::ANIMATION::MOVE_FWD, std::make_unique<Judgement_TransitionReady>(enemy));
					}
				}
				// ステート設定
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
				// 円のコライダー
				{
					CircleCollisionComponent::CollisionParam param{};
					param.collision_type = COLLISION_TYPE::DEFENDER;
					auto collision = enemy->AddComponent<CircleCollisionComponent>(param);
				}
				// トランスフォーム設定
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

				// キャラクターステータス
				{
					CharacterComponent::CharacterParam param{};
					param.max_hp = 10;
					param.hp = 5;
					enemy->AddComponent<CharacterComponent>(param);
				}

				// シェーダー設定
				auto shader_component =
					enemy->AddComponent<InstancingModelShaderComponent>(this->instancing_model_shader.get());
			
				if (GameObject* game_object = GameObject::Instance())
				{
					game_object->SetEnemy(enemy);
				}
			}
		}

#ifdef _DEBUG	// デバッグ用object
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
	// オーディオのリセット
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
	// スペースキーでゲーム画面に遷移(仮)
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		AudioParam param{};
		param.filename = "Data/Debug/Audio/SE.wav";
		Audio::Instance()->Play(param);
	}
#endif // DEBUG

	// ゲーム状態を処理する
	ProcessGameState();
}

void SceneGame::Render()
{
	Graphics* graphics = Graphics::Instance();
	graphics->PrepareRenderTargets(DirectX::XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f));

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

		DebugManager::Instance()->GetDebugPrimitiveRenderer()->Render();
	}
#endif // _DEBUG

	// 2Dスプライト描画
	{
		sprite_shader->Render();
	}
}

void SceneGame::ReStart()
{
	object_manager.ReStart();

	// ゲーム状態をデフォルトに設定
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

	// ゲーム状態を取得
	const GameData::GameStatus& game_status = game_data->GetGameStatus();

	switch (game_status)
	{
	case GameData::GameStatus::DEFEAT:	// プレイヤーの敗北
	{
		SceneManager* scene_manager = SceneManager::Instance();
		if (!scene_manager) return;

		scene_manager->ChangeScene(new SceneResult);
		break;
	}
	case GameData::GameStatus::VICTORY:	// プレイヤーの勝利
	{
		SceneManager* scene_manager = SceneManager::Instance();
		if (!scene_manager) return;

		scene_manager->ChangeScene(new SceneLoading(new SceneResult));
		break;
	}
	case GameData::GameStatus::RETURN_TO_TITLE: // タイトルに戻る
	{
		SceneManager* scene_manager = SceneManager::Instance();
		if (!scene_manager) return;

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

void SceneGame::PlayerVsEnemy()
{
	// 当たり判定
	if (GameObject* game_object = GameObject::Instance())
	{
		// プレイヤー取得
		const auto& player = game_object->GetPlayer();
		if (!player) return;
		if (!player->GetIsActive()) return;	// プレイヤーが非アクティブなら処理しない

		// プレイヤーの攻撃用オブジェクト取得
		const auto& attack_object = player->FindChildObject(MyHash("AttackObject"));

		const auto& player_circle = attack_object->GetComponent<CircleCollisionComponent>();
		if (!player_circle) return;

		// ヒットフラグの更新
		player_circle->SetHitOldFlag(player_circle->GetHitFlag());
		player_circle->SetHitFlag(false);
		if (!player_circle->GetIsActive()) return;

		// 敵取得
		for (const std::shared_ptr<Object>& enemy : game_object->GetEnemyPool())
		{
			if (!enemy) continue;
			if (!enemy->GetIsActive()) continue;
			const auto& enemy_circle = enemy->GetComponent<CircleCollisionComponent>();
			if (!enemy_circle) return;
			// ヒットフラグの更新
			enemy_circle->SetHitOldFlag(enemy_circle->GetHitFlag());
			enemy_circle->SetHitFlag(false);
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
				// 旧処理(最終的に削除する)
				{
					// 衝突した場合、攻撃側と防御側にヒットフラグを設定
					player_circle->SetHitFlag(true);
					enemy_circle->SetHitFlag(true);

					// 攻撃側のリザルト設定
					player_hit_result.hit_object_Wptr = enemy_circle->GetOwner();
					player_circle->SetHitResult(player_hit_result);

					// 防御側のリザルト設定
					enemy_hit_result.hit_object_Wptr = player_circle->GetOwner();
					enemy_circle->SetHitResult(enemy_hit_result);
				}

				// 接触処理
				player_circle->OnCollision(player_circle->GetOwner());
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