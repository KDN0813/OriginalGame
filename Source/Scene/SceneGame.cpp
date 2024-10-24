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
#include "Component/EnemyComponent.h"
#include "Component/StateMachineComponent.h"
#include "Component/CircleCollisionComponent.h"
#include "Component/CameraControllerComponent.h"
#include "Component/SpriteComponent.h"

#include "StateMachine/TransitionJudgementDerived.h"
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
			auto stage = object_manager.Create();
			stage->SetName("Stage");
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

			// GameObjectに設定
			GameObject::Instance()->SetGameObject(GameObject::OBJECT_TYPE::STAGE, stage);
		}

		// プレイヤー
		{
			auto player = object_manager.Create();
			player->SetName("Player");
			auto model = player->AddComponent<ModelComponent>("Data/Model/Player/Player.mdl");
			//auto model = player->AddComponent<AnimatedInstancedModelComponent>(device, "Data/Model/Player/Player.mdl");
			// アニメーション設定
			{
				ModelAnimationComponent::AnimationParam param{};
				param.current_animation_index = PlayerCT::ANIMATION::IDLE;
				param.animation_loop_flag = true;

				auto model_animation = player->AddComponent<ModelAnimationComponent>(param , "Data/Model/Player/Player.mdl");
				// 待機
				model_animation->SetAnimationState(PlayerCT::ANIMATION::IDLE, true);
				model_animation->AddAnimationTransition(PlayerCT::ANIMATION::IDLE, PlayerCT::ANIMATION::MOVE_FWD, std::make_unique<Judgement_Move>(player),0.2f);
				model_animation->AddAnimationTransition(PlayerCT::ANIMATION::IDLE, PlayerCT::ANIMATION::ATTACK01, std::make_unique<Judgement_ButtonDown>(player, GamePad::BTN_X), 0.2f);

				// 前方移動
				model_animation->SetAnimationState(PlayerCT::ANIMATION::MOVE_FWD, true);
				model_animation->AddAnimationTransition(PlayerCT::ANIMATION::MOVE_FWD, PlayerCT::ANIMATION::IDLE, std::make_unique<Judgement_Move>(player, true), 0.2f);
				model_animation->AddAnimationTransition(PlayerCT::ANIMATION::MOVE_FWD, PlayerCT::ANIMATION::ATTACK01, std::make_unique<Judgement_ButtonDown>(player, GamePad::BTN_X), 0.2f);

				// 攻撃01
				model_animation->SetAnimationState(PlayerCT::ANIMATION::ATTACK01, false);
				model_animation->AddAnimationTransition(PlayerCT::ANIMATION::ATTACK01, PlayerCT::ANIMATION::IDLE, std::make_unique<Judgement_TransitionReady>(player, false, true), 0.2f);
			}
			auto state_machine = player->AddComponent<StateMachineComponent>();
			{
				auto idle_state = state_machine->RegisterState<IdelState>();
				idle_state->AddStateTransition(std::make_unique<StateTransitionInfo>("AttackState", std::make_unique<Judgement_ButtonDown>(player, GamePad::BTN_X)), StateBase::JudgementUpdatePhase::PostUpdate);
				auto attack_state = state_machine->RegisterState<AttackState>();
				//attack_state->AddStateTransition(std::make_unique<StateTransitionInfo>("IdelState", std::make_unique<Judgement_ButtonDown>(player, GamePad::BTN_Y)), StateBase::JudgementUpdatePhase::PostUpdate);

				state_machine->SetDefaultState("IdelState");
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
			// 円のコライダー
			{
				CircleCollisionComponent::CollisionParam param{};
				param.collision_type = COLLISION_TYPE::ATTACKER;
				param.self_type = OBJECT_TYPE::PLAYER;
				param.target_type = OBJECT_TYPE::ENEMY;
				param.default_active_flag = false;
				auto collision = player->AddComponent<CircleCollisionComponent>(param);

				CollisionManager::Instance()->GetCircleCollider()->AddCircle(collision);
			}

			// 子オブジェクト設定
			{
				std::shared_ptr<Object> object = player->AddChildren();
				object->SetName("player child");
				object->AddComponent<ModelComponent>("Data/Debug/Model/Jammo/Jammo.mdl");

				// トランスフォーム設定
				{
					Transform3DComponent::Transform3DParam param{};
					param.local_position = DirectX::XMFLOAT3(100.0f, 0.0f, 0.0f);
					param.local_scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

					auto transform = object->AddComponent<Transform3DComponent>(param);
				}
				// ムーブメント設定
				{
					auto movement = object->AddComponent<MovementComponent>(MovementComponent::MovementParam());
				}
				// シェーダー設定
				auto shader_component =
					object->AddComponent<ModelShaderComponent>(model_shader.get());
			}

			// GameObjectに設定
			GameObject::Instance()->SetGameObject(GameObject::OBJECT_TYPE::PLAYER, player);
		}


		// デスカメラ
		{
			auto death_camera = object_manager.Create();
			CameraManager* camera_manager = CameraManager::Instance();
			if (camera_manager)
			{
				auto camera = death_camera->AddComponent<CameraComponent>(camera_manager->GetCamera(CAMERA_TYPE::DEATH));
			}
		}

		// 敵
		{
			float territory_range = 45.0f;
			for (int i = 0; i < 1000; ++i)
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
						model->SetAnimationState(EnemyCT::ANIMATION::MOVE_FWD, true);
						model->AddAnimationTransition(EnemyCT::ANIMATION::MOVE_FWD, EnemyCT::ANIMATION::IDLE_BATTLE, std::make_unique<Judgement_IsAtTarget>(enemy));
						model->SetAnimationState(EnemyCT::ANIMATION::IDLE_BATTLE, true);
						model->AddAnimationTransition(EnemyCT::ANIMATION::IDLE_BATTLE, EnemyCT::ANIMATION::MOVE_FWD, std::make_unique<Judgement_IdleFinished>(enemy));
					}
				}
				// ステート設定
				auto state_machine = enemy->AddComponent<StateMachineComponent>();
				{
					auto idle_state = state_machine->RegisterState<IdelState>();
					idle_state->AddStateTransition(std::make_unique<StateTransitionInfo>("WanderState", std::make_unique<Judgement_IdleFinished>(enemy)), StateBase::JudgementUpdatePhase::PostUpdate);
					auto wander_state = state_machine->RegisterState<WanderState>();
					wander_state->AddStateTransition(std::make_unique<StateTransitionInfo>("IdelState", std::make_unique<Judgement_IsAtTarget>(enemy)), StateBase::JudgementUpdatePhase::PostUpdate);

					state_machine->SetDefaultState("WanderState");
				}
				// 円のコライダー
				{
					CircleCollisionComponent::CollisionParam param{};
					param.collision_type = COLLISION_TYPE::DEFENDER;
					param.self_type = OBJECT_TYPE::ENEMY;
					param.target_type = OBJECT_TYPE::PLAYER;
					auto collision = enemy->AddComponent<CircleCollisionComponent>(param);
					CollisionManager::Instance()->GetCircleCollider()->AddCircle(collision);
				}
				// トランスフォーム設定
				{
					float offset = 2.0f;
					float theta = MyMathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
					float range = MyMathf::RandomRange(0.0f, territory_range);

					Transform3DComponent::Transform3DParam param{};
					param.local_position  = 
					{
						sinf(theta) * range,
						0.0f,
						cosf(theta) * range ,
					};
					param.local_scale = DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f);

					auto transform = enemy->AddComponent<Transform3DComponent>(param);
				}

				// シェーダー設定
				auto shader_component =
					enemy->AddComponent<InstancingModelShaderComponent>(this->instancing_model_shader.get());
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

	CollisionManager::Instance()->Update();

	Audio::Instance()->Update();

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

#ifdef _DEBUG

void SceneGame::DebugDrawGUI()
{
	this->object_manager.DrawDebugGUI();

	// シェーダー
	DrawShaderImGui();

	// コリジョン
	this->collision_manager.DrawDebugGUI();
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