#include "EnemyConstant.h"
#include "Object\Object.h"
#include "System\MyMath\MyMathf.h"

#include "Shader\InstanceModelShader.h"
#include "StateMachine\EnemyStateDerived.h"
#include "Object\Constant\StageConstant.h"

#include "Component\EnemyComponent.h"
#include "Component\DamageComponent.h"
#include "Component\FinalUpdateEnemyComponent.h"
#include "Component\MovementComponent.h"
#include "Component\InstancedModelWithAnimationComponent.h"
#include "Component\TransformComponent.h"
#include "Component\CharacterComponent.h"
#include "Component\InstancingModelShaderComponent.h"
#include "Component\CircleCollisionComponent.h"
#include "Component\StateMachineComponent.h"
#include "Component\EnemySpawnerComponent.h"

const MyHash EnemyConstant::ATTACK_OBJECT_NAME = MyHash("EnemyAttackObject");

const std::shared_ptr<Object>& EnemyConstant::CreateEnemy(const std::shared_ptr<Object>& enemy, const std::shared_ptr<Object>& chain_score_counter_object, const EnemyConfig& config)
{
	// コリジョンに設定するコンポーネントは事前に作成しておく
	std::shared_ptr<EnemyComponent> enemy_component;

	// エネミーコンポーネント設定
	{
		EnemyComponent::EnemyParam param{};
		param.spawn_point = config.spawn_point;
		param.add_special_point = 1.0f;
		param.move_speed = config.move_speed;
		param.add_score = 1;
		enemy_component = enemy->AddComponent<EnemyComponent>(param);

		enemy_component->SetChainScoreCounterObject(chain_score_counter_object);
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
		param.anime_index = ANIMATION::IDLE_BATTLE;
		param.anime_loop = true;
		param.anime_play = true;
		param.base_color = config.base_color;

		auto model = enemy->AddComponent<InstancedModelWithAnimationComponent>(param, "Data/Model/ChestMonster/ChestMonster.mdl");
	}
	// トランスフォーム設定
	{
		Transform3DComponent::Transform3DParam param{};
		param.local_position = config.spawn_point;
		param.local_scale = DirectX::XMFLOAT3(0.015f, 0.015f, 0.015f);

		auto transform = enemy->AddComponent<Transform3DComponent>(param);
	}

	// キャラクターステータス
	{
		CharacterComponent::CharacterParam param{};
		param.max_hp = 2;
		param.hp = 2;
		param.radius = 1.5f;
		enemy->AddComponent<CharacterComponent>(param);
	}

	// シェーダー設定
	{
		InstancingModelShader::Instance instancing_model_shader = InstancingModelShader::GetInstance();
		const auto& shader_component =
			enemy->AddComponent<InstancingModelShaderComponent>(instancing_model_shader.Get());
	}

	// 円のコライダー
	{
		CircleCollisionComponent::CollisionParam param{};
		param.collision_type = COLLISION_OBJECT_TYPE::ENEMY_DEFENSE;
		auto collision = enemy->AddComponent<CircleCollisionComponent>(param);
		collision->AddCollisionComponent(enemy_component);
	}
	// ステートマシン
	{
		// ステートマシン設定
		auto state_machine = enemy->AddComponent<StateMachineComponent>();
		{
			// ステートの追加
			state_machine->RegisterState<EnemySpawnState>("SpawnState");
			state_machine->RegisterState<EnemyIdleState>("IdleState");
			state_machine->RegisterState<EnemyWanderState>("WanderState");
			state_machine->RegisterState<EnemyChaseState>("ChaseState");
			state_machine->RegisterState<EnemyAttackState>("AttackState");
			state_machine->RegisterState<EnemyDamageState>("DamageState");
			state_machine->RegisterState<EnemyDeadState>("DeadState");
			state_machine->RegisterState<EnemyDeadIdleState>("DeadIdleState");

			state_machine->SetDefaultState("SpawnState");	// デフォルトステートの設定
		}
	}
	// FinalUpdateEnemyComponent
	{
		enemy->AddComponent<FinalUpdateEnemyComponent>();
	}

	// 子オブジェクト
	{
		// 攻撃用オブジェクト
		{
			std::shared_ptr<Object> enemy_attack_object = enemy->CreateChildObject();
			enemy_attack_object->SetName(ATTACK_OBJECT_NAME.GetString().c_str());
			// トランスフォーム設定
			{
				Transform3DComponent::Transform3DParam param{};
				param.local_position = DirectX::XMFLOAT3(0.0f, 0.0f, 100.0f);
				param.local_scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

				auto child_transform = enemy_attack_object->AddComponent<Transform3DComponent>(param);
			}
			// 円のコライダー
			{
				CircleCollisionComponent::CollisionParam param{};
				param.collision_type = COLLISION_OBJECT_TYPE::ENEMY_ATTACK;
				param.radius = 2.5f;
				param.default_active_flag = false;
				auto child_collision = enemy_attack_object->AddComponent<CircleCollisionComponent>(param);

				// 接触時処理するコンポーネントの追加
				{
					child_collision->AddCollisionComponent(enemy_component);
				}
			}
			// ダメージ用コンポーネント
			{
				DamageComponent::DamageParam param{};
				param.damage_amount = 1;

				enemy_attack_object->AddComponent<DamageComponent>(param);
			}
		}
	}

	return enemy;
}

const std::shared_ptr<Object>& EnemyConstant::CreateEnemySpawner(const std::shared_ptr<Object>& object, const std::shared_ptr<ObjectManager>& object_manager, const std::shared_ptr<Object>& chain_score_counter_object)
{
	// Transform3DComponent
	{
		Transform3DComponent::Transform3DParam param{};
		object->AddComponent<Transform3DComponent>(param);
	}
	// EnemySpawnerComponent
	{
		EnemySpawnerComponent::Param param{};
		param.create_cool_timer = 0.0f;
		param.create_cool_time_max = 5.0f;
		param.create_cool_time_min = 3.0f;

		param.max_spawn_dist = 100.0f;
		param.min_spawn_dist = 20.0f;

#ifdef _DEBUG
		param.min_enemies_per_spawn = 10;
		param.max_enemies_per_spawn = 20;
#else
		param.min_enemies_per_spawn = 80;
		param.max_enemies_per_spawn = 150;
#endif // _DEBUG
		const auto& enemy_spawner = object->AddComponent<EnemySpawnerComponent>(param);
		enemy_spawner->SetObjectManager(object_manager);
		enemy_spawner->SetChainScoreCounterObject(chain_score_counter_object);
	}
	return object;
}
