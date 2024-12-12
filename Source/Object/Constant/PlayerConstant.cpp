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
	// �R���W�����ɐݒ肷��R���|�[�l���g�͎��O�ɍ쐬���Ă���
	std::shared_ptr<PlayerComponent> player_component;// �v���C���[�R���|�[�l���g

	// ���f���ݒ�
	{
		auto model = player->AddComponent<ModelComponent>("Data/Debug/Model/Player/Player.mdl");
	}
	// �A�j���[�V�����ݒ�
	{
		ModelAnimationControlComponent::InitAnimeParam param{};
		param.init_anime_index = PlayerConstant::IDLE;
		param.init_anime_loop = true;
		auto model_animation = player->AddComponent<ModelAnimationControlComponent>(param);
	}
	// �X�e�[�g�}�V���ݒ�
	auto state_machine = player->AddComponent<StateMachineComponent>();
	{
		// �X�e�[�g�̒ǉ�
		state_machine->RegisterState<PlayerIdleState>();
		state_machine->RegisterState<PlayerMoveState>();
		state_machine->RegisterState<PlayerAttackState>();
		state_machine->RegisterState<PlayerSpinAttackState>();

		state_machine->SetDefaultState("PlayerIdleState");	// �f�t�H���g�X�e�[�g�̐ݒ�
	}

	// �g�����X�t�H�[���ݒ�
	{
		Transform3DComponent::Transform3DParam param{};
		param.local_scale = DirectX::XMFLOAT3(0.018f, 0.018f, 0.018f);
		param.local_position = DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f);
		auto transform = player->AddComponent<Transform3DComponent>(param);
	}
	// ���[�u�����g�ݒ�
	{
		MovementComponent::MovementParam param{};
		param.max_accelerationXZ = 7.0f;
		param.is_stage_raycas = true;
		auto movement = player->AddComponent<MovementComponent>(param);
	}
	// �v���C���[�R���|�[�l���g�쐬
	{
		player_component = player->AddComponent<PlayerComponent>(PlayerComponent::PlayerParam());
	}
	// �V�F�[�_�[�ݒ�
	{
		auto shader_component = player->AddComponent<ModelShaderComponent>(model_shader.get());
	}
	// �d��
	{
		player->AddComponent<GravityComponent>(GravityComponent::GravityParam());
	}
	// �L�����N�^�[�X�e�[�^�X
	{
		CharacterComponent::CharacterParam param{};
		param.max_hp = 100;
		param.hp = 50;
		player->AddComponent<CharacterComponent>(param);
	}


	// �q�I�u�W�F�N�g
	{
		// �v���C���[�J����
		{
			std::shared_ptr<Object> player_camera_object = player->CreateChildObject();
			player_camera_object->SetName("PlayerCamera");
			// �J�����ݒ�
			{
				if (CameraManager::Instance camera_manager = CameraManager::GetInstance(); camera_manager.Get())
				{
					auto camera = player_camera_object->AddComponent<CameraComponent>(camera_manager->GetCamera(CAMERA_TYPE::MAIN));
				}
			}
			// �J�����R���g���[���[�ݒ�
			{
				CameraControllerGamepad::CameraControllerParam param{};
				player_camera_object->AddComponent<CameraControllerGamepad>(param);
			}
			// �g�����X�t�H�[���ݒ�
			{
				Transform3DComponent::Transform3DParam param{};
				player_camera_object->AddComponent<Transform3DComponent>(param);
			}
		}
		// �v���C���[�̍U������p�I�u�W�F�N�g
		{
			std::shared_ptr<Object> player_attack_object = player->CreateChildObject();
			player_attack_object->SetName("AttackObject");
			// �g�����X�t�H�[���ݒ�
			{
				Transform3DComponent::Transform3DParam param{};
				param.local_position = DirectX::XMFLOAT3(0.0f, 0.0f, 100.0f);
				param.local_scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

				auto child_transform = player_attack_object->AddComponent<Transform3DComponent>(param);
			}
			// ���[�u�����g�ݒ�
			{
				auto child_movement = player_attack_object->AddComponent<MovementComponent>(MovementComponent::MovementParam());
			}
			// �~�̃R���C�_�[
			{
				CircleCollisionComponent::CollisionParam param{};
				param.collision_type = COLLISION_OBJECT_TYPE::PLAYER;
				param.radius = 2.0f;
				param.default_active_flag = false;
				auto child_collision = player_attack_object->AddComponent<CircleCollisionComponent>(param);

				// �ڐG����������R���|�[�l���g�̒ǉ�
				{
					child_collision->AddCollisionEntercomponent(player_component);
				}
			}
		}
		// ��]�U���p�����蔻��I�u�W�F�N�g
		{
			std::shared_ptr<Object> player_attack_object = player->CreateChildObject();
			player_attack_object->SetName("SpinAttackObject");
			// �g�����X�t�H�[���ݒ�
			{
				Transform3DComponent::Transform3DParam param{};
				param.local_position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
				param.local_scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

				auto child_transform = player_attack_object->AddComponent<Transform3DComponent>(param);
			}
			// �~�̃R���C�_�[
			{
				CircleCollisionComponent::CollisionParam param{};
				param.collision_type = COLLISION_OBJECT_TYPE::PLAYER;
				param.radius = 5.0f;
				param.default_active_flag = false;
				auto child_collision = player_attack_object->AddComponent<CircleCollisionComponent>(param);

				// �ڐG����������R���|�[�l���g�̒ǉ�
				{
					child_collision->AddCollisionEntercomponent(player_component);
				}
			}
		}
	}
}