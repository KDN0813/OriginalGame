#include "PlayerConstant.h"
#include "Object\Object.h"

#include "Camera\CameraManager.h"
#include "Shader\ModelShader.h"
#include "GridObjectManager\GridObjectManager.h"
#include "Graphics\Graphics.h"
#include "Object\Constant\StageConstant.h"

#include "Component/PlayerComponent.h"
#include "Component/ModelComponent.h"
#include "Component/ModelAnimationControlComponent.h"
#include "Component/StateMachineComponent.h"
#include "Component/TransformComponent.h"
#include "Component/MovementComponent.h"
#include "Component/CharacterComponent.h"
#include "Component/ModelShaderComponent.h"
#include "Component/CameraControllerComponent.h"
#include "Component\CameraComponent.h"
#include "Component/CircleCollisionComponent.h"
#include "Component/DamageComponent.h"

#include "Component/Transform2DComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/SpinAttackTimeUIComponent.h"

#include "StateMachine\PlayerStateDerived.h"

const MyHash PlayerConstant::PLAYER_CAMERA_NAME = MyHash("PlayerCamera");
const MyHash PlayerConstant::DEATH_CAMERA_NAME = MyHash("DeathCamera");
const MyHash PlayerConstant::ATTACK01_OBJECT_NAME = MyHash("Attack01Object");
const MyHash PlayerConstant::ATTACK02_OBJECT_NAME = MyHash("Attack02Object");
const MyHash PlayerConstant::DEFENSE_OBJECT_NAME = MyHash("DefenseObject");
const MyHash PlayerConstant::SPIN_ATTACK_OBJECT_NAME = MyHash("SpinAttackObject");
const MyHash PlayerConstant::SPIN_ATTACK_TIME_UI_NAME = MyHash("SpinAttackTimeUI");

const std::shared_ptr<Object>& PlayerConstant::CreatePlayer(const std::shared_ptr<Object>& player)
{
	// コリジョンに設定するコンポーネントは事前に作成しておく
	std::shared_ptr<PlayerComponent> player_component;// プレイヤーコンポーネント

	// モデル設定
	{
		auto model = player->AddComponent<ModelComponent>("Data/Model/Player/Player.mdl");
	}
	// アニメーション設定
	{
		ModelAnimationControlComponent::InitAnimeParam param{};
		param.init_anime_index = PlayerConstant::IDLE;
		param.init_anime_loop = true;
		param.sub_part_root_name_vec.emplace_back("thigh_l");
		param.sub_part_root_name_vec.emplace_back("thigh_r");
		const auto& model_animation = player->AddComponent<ModelAnimationControlComponent>(param);
	}
	// ステートマシン設定
	auto state_machine = player->AddComponent<StateMachineComponent>();
	{
		// ステートの追加
		state_machine->RegisterState<PlayerIdleState>("IdleState");
		state_machine->RegisterState<PlayerMoveState>("MoveState");
		state_machine->RegisterState<PlayerAttackState>("AttackState");
		state_machine->RegisterState<PlayerAttackComboState>("AttackComboState");
		state_machine->RegisterState<PlayerAttackHoldState>("AttackHoldState");
		state_machine->RegisterState<PlayerSpinAttackStartState>("SpinAttackStartState");
		state_machine->RegisterState<PlayerSpinAttackSpinLoopState>("SpinAttackSpinLoopState");
		state_machine->RegisterState<PlayerDamageState>("DamageState");
		state_machine->RegisterState<PlayerDeadState>("DeadState");
		state_machine->RegisterState<PlayerDeadIdleState>("DeadIdleState");

		state_machine->SetDefaultState("IdleState");	// デフォルトステートの設定
	}

	// トランスフォーム設定
	{
		const float half_cell_size =  GridObjectManager::HALF_CELL_SIZE;

		Transform3DComponent::Transform3DParam param{};
		param.local_scale = DirectX::XMFLOAT3(0.018f, 0.018f, 0.018f);
		param.local_position = DirectX::XMFLOAT3(half_cell_size, StageConstant::STAGE_FLOOR_Y, half_cell_size);
		param.local_angle = {};
		const auto& transform = player->AddComponent<Transform3DComponent>(param);
	}
	// ムーブメント設定
	{
		MovementComponent::MovementParam param{};
		param.max_accelerationXZ = 100.0f;
		param.is_stage_raycast = false;
		auto movement = player->AddComponent<AnimatedMovementComponent>(param);
	}
	// プレイヤーコンポーネント作成
	{
		PlayerComponent::PlayerParam param{};
		param.move_speed = 15.0f;
		param.special_point_max = 40.0f;
		param.spin_attack_time = 1.5f;
		param.spin_attack_use_gage_count = 1;
		param.gauge_count_max = SPECIAL_GAGE_MAX;
		param.attack_move_rate = 0.3f;
		player_component = player->AddComponent<PlayerComponent>(param);
	}
	// シェーダー設定
	{
		ModelShader::Instance model_shader = ModelShader::GetInstance();
		const auto& shader_component = player->AddComponent<ModelShaderComponent>(model_shader.Get());
	}
	// キャラクターステータス
	{
		CharacterComponent::CharacterParam param{};
		param.max_hp = 100;
		param.hp = 100;
		param.radius = 1.5f;
		player->AddComponent<CharacterComponent>(param);
	}

	// ゲージUI用2Dトランスフォーム
	{
		Transform2DComponent::Transform2DParam param{};
		player->AddComponent<Transform2DComponent>(param);
	}


	// 子オブジェクト
	{
		// プレイヤーカメラ
		{
			std::shared_ptr<Object> player_camera_object = player->CreateChildObject(PLAYER_CAMERA_NAME.GetString().c_str());
			// カメラ設定
			{
				CameraManager::Instance camera_manager = CameraManager::GetInstance();
				auto camera = player_camera_object->AddComponent<CameraComponent>(camera_manager->GetCamera(CAMERA_TYPE::MAIN));

				CameraComponent::CameraParam param{};
				param.rotateX = 0.8f;
				param.range = 40.0f;
				param.fovY = DirectX::XMConvertToRadians(45.0f);
				// グラフィックスからアスペクト比を計算
				{
					Graphics::Instance graphics = Graphics::GetInstance();
					param.aspect = graphics->GetScreenWidth() / graphics->GetScreenHeight();
				}
				param.nearZ = 0.1f;
				param.farZ = 1000.0f;
				camera->SetDefaultParam(param);
			}
			// カメラコントローラー設定
			{
				player_camera_object->AddComponent<CameraControllerDefault>();
			}
			// トランスフォーム設定
			{
				Transform3DComponent::Transform3DParam param{};
				player_camera_object->AddComponent<Transform3DComponent>(param);
			}
		}
		// デスカメラ
		{
			std::shared_ptr<Object> death_camera_object = player->CreateChildObject(DEATH_CAMERA_NAME.GetString().c_str());

			// カメラ設定
			{
				{
					CameraManager::Instance camera_manager = CameraManager::GetInstance();
					const auto camera_component = death_camera_object->AddComponent<CameraComponent>(camera_manager->GetCamera(CAMERA_TYPE::DEATH));
				
					camera_component->SetRange(15.0f);
					camera_component->SetRotateX(DirectX::XMConvertToRadians(85.0f));
				}
			}
			// トランスフォーム設定
			{
				Transform3DComponent::Transform3DParam param{};
				death_camera_object->AddComponent<Transform3DComponent>(param);
			}
			// カメラコントローラー設定
			{
				CameraControllerDeathComponent::CameraControllerParam param{};
				param.rotation_speed = DirectX::XMConvertToRadians(5.0f);
				death_camera_object->AddComponent<CameraControllerDeathComponent>(param);
			}
		}

		// プレイヤーの攻撃01判定用オブジェクト
		{
			std::shared_ptr<Object> player_attack_object = player->CreateChildObject();
			player_attack_object->SetName(ATTACK01_OBJECT_NAME.GetString().c_str());
			// トランスフォーム設定
			{
				Transform3DComponent::Transform3DParam param{};
				param.local_position = DirectX::XMFLOAT3(0.0f, 0.0f, 190.0f);
				param.local_scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

				auto child_transform = player_attack_object->AddComponent<Transform3DComponent>(param);
			}
			// 円のコライダー
			{
				CircleCollisionComponent::CollisionParam param{};
				param.collision_type = COLLISION_OBJECT_TYPE::PLAYER_ATTACK;
				param.radius = 3.0f;
				param.default_active_flag = false;
				auto child_collision = player_attack_object->AddComponent<CircleCollisionComponent>(param);

				// 接触時処理するコンポーネントの追加
				{
					child_collision->AddCollisionComponent(player_component);
				}
			}
			// ダメージデータ
			{
				DamageComponent::DamageParam param{};
				param.damage_amount = 1;
				player_attack_object->AddComponent<DamageComponent>(param);
			}
		}
		// プレイヤーの攻撃02判定用オブジェクト
		{
			std::shared_ptr<Object> player_attack_object = player->CreateChildObject();
			player_attack_object->SetName(ATTACK02_OBJECT_NAME.GetString().c_str());
			// トランスフォーム設定
			{
				Transform3DComponent::Transform3DParam param{};
				param.local_position = DirectX::XMFLOAT3(0.0f, 0.0f, 236.0f);
				param.local_scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

				auto child_transform = player_attack_object->AddComponent<Transform3DComponent>(param);
			}
			// 円のコライダー
			{
				CircleCollisionComponent::CollisionParam param{};
				param.collision_type = COLLISION_OBJECT_TYPE::PLAYER_ATTACK;
				param.radius = 3.5f;
				param.default_active_flag = false;
				auto child_collision = player_attack_object->AddComponent<CircleCollisionComponent>(param);

				// 接触時処理するコンポーネントの追加
				{
					child_collision->AddCollisionComponent(player_component);
				}
			}
			// ダメージデータ
			{
				DamageComponent::DamageParam param{};
				param.damage_amount = 3;
				player_attack_object->AddComponent<DamageComponent>(param);
			}
		}
		// 回転攻撃用当たり判定オブジェクト
		{
			std::shared_ptr<Object> player_attack_object = player->CreateChildObject();
			player_attack_object->SetName(SPIN_ATTACK_OBJECT_NAME.GetString().c_str());
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
				param.collision_type = COLLISION_OBJECT_TYPE::PLAYER_ATTACK;
				param.radius = 5.0f;
				param.default_active_flag = false;
				auto child_collision = player_attack_object->AddComponent<CircleCollisionComponent>(param);

				// 接触時処理するコンポーネントの追加
				{
					child_collision->AddCollisionComponent(player_component);
				}
			}
			// ダメージデータ
			{
				DamageComponent::DamageParam param{};
				param.damage_amount = 4;
				param.invincible_time = 1.0f;
				param.get_point_rate = 0;
				player_attack_object->AddComponent<DamageComponent>(param);
			}
		}
		// ヒットボックス
		{
			std::shared_ptr<Object> player_attack_object = player->CreateChildObject(DEFENSE_OBJECT_NAME.GetString().c_str());
			// トランスフォーム設定
			{
				Transform3DComponent::Transform3DParam param{};
				param.local_position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

				auto child_transform = player_attack_object->AddComponent<Transform3DComponent>(param);
			}
			// 円のコライダー
			{
				CircleCollisionComponent::CollisionParam param{};
				param.collision_type = COLLISION_OBJECT_TYPE::PLAYER_DEFENSE;
				param.radius = 2.0f;
				auto child_collision = player_attack_object->AddComponent<CircleCollisionComponent>(param);

				// 接触時処理するコンポーネントの追加
				{
					child_collision->AddCollisionComponent(player_component);
				}
			}
		}
		// 回転攻撃時のゲージUI
		{
			std::shared_ptr<Object> spin_attack_time_UI_object = player->CreateChildObject(SPIN_ATTACK_TIME_UI_NAME.GetString().c_str());
			spin_attack_time_UI_object->SetIsActive(false);

			// トランスフォーム設定
			{
				Transform2DComponent::Transform2DParam param{};
				param.local_position = DirectX::XMFLOAT2(0.501f, 0.425f);
				param.local_scale = { 0.05f,0.01f };

				auto child_transform = spin_attack_time_UI_object->AddComponent<Transform2DComponent>(param);
			}
			// スプライト
			{
				BaseSpriteComponent::SpriteParam sprite_param{};
				sprite_param.color = { 1.0f,0.0f, 0.0f, 1.0f };
				sprite_param.center_type = Sprite::CENTER_TYPE::CENTER;
				const auto& sprite = spin_attack_time_UI_object->AddComponent<SpriteComponent>(sprite_param);
			}
			// UIの更新処理
			{
				spin_attack_time_UI_object->AddComponent<SpinAttackTimeUIComponent>();
			}
		}
	}

	return player;
}