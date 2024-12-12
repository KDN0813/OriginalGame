#include "PlayerConstant.h"
#include "Object\Object.h"

#include "Camera\CameraManager.h"

#include "Component/PlayerComponent.h"
#include "Component/ModelComponent.h"
#include "Component/ModelAnimationControlComponent.h"
#include "Component/StateMachineComponent.h"
#include "Component/TransformComponent.h"
#include "Component/MovementComponent.h"
#include "Component/CharacterComponent.h"
#include "Component/GravityComponent.h"
#include "Component/ModelShaderComponent.h"
#include "Component/CameraControllerComponent.h"
#include "Component/CircleCollisionComponent.h"

#include "StateMachine\PlayerStateDerived.h"

void PlayerConstant::CreatePlayer(const std::shared_ptr<Object>& player)
{
	// コリジョンに設定するコンポーネントは事前に作成しておく
	std::shared_ptr<PlayerComponent> player_component;// プレイヤーコンポーネント

	// モデル設定
	{
		auto model = player->AddComponent<ModelComponent>("Data/Debug/Model/Player/Player.mdl");
	}
	// アニメーション設定
	{
		ModelAnimationControlComponent::InitAnimeParam param{};
		param.init_anime_index = PlayerConstant::IDLE;
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
	}
}