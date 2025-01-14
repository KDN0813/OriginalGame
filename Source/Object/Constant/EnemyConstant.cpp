#include "EnemyConstant.h"
#include "Object\Object.h"
#include "System\MyMath\MyMathf.h"

#include "Shader\InstanceModelShader.h"

#include "Component\EnemyComponent.h"
#include "Component\MovementComponent.h"
#include "Component\InstancedModelWithAnimationComponent.h"
#include "Component\TransformComponent.h"
#include "Component\CharacterComponent.h"
#include "Component\InstancingModelShaderComponent.h"
#include "Component\CircleCollisionComponent.h"

const std::shared_ptr<Object>& EnemyConstant::CreateEnemy(const std::shared_ptr<Object>& enemy)
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
		param.anime_index = ANIMATION::IDLE_BATTLE;
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
		param.max_invincible_timer = 0.0f;
		enemy->AddComponent<CharacterComponent>(param);
	}

	// シェーダー設定
	if(InstancingModelShader::Instance instancing_model_shader = InstancingModelShader::GetInstance(); instancing_model_shader.Get())
	{
		auto shader_component =
			enemy->AddComponent<InstancingModelShaderComponent>(instancing_model_shader.Get());
	}

	// 円のコライダー
	{
		CircleCollisionComponent::CollisionParam param{};
		param.collision_type = COLLISION_OBJECT_TYPE::ENEMY;
		auto collision = enemy->AddComponent<CircleCollisionComponent>(param);
		collision->AddCollisionComponent(enemy_component);
	}

	return enemy;
}
