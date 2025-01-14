#include "PlayerConstant.h"
#include "Object\Object.h"

#include "Camera\CameraManager.h"
#include "Shader\ModelShader.h"

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
#include "Component\CameraComponent.h"
#include "Component/CircleCollisionComponent.h"

#include "StateMachine\PlayerStateDerived.h"

const MyHash PlayerConstant::PLAYER_CAMERA_NAME = MyHash("PlayerCamera");
const MyHash PlayerConstant::ATTACK_OBJECT_NAME = MyHash("AttackObject");
const MyHash PlayerConstant::SPIN_ATTACK_OBJECT_NAME = MyHash("SpinAttackObject");

const std::shared_ptr<Object>& PlayerConstant::CreatePlayer(const std::shared_ptr<Object>& player)
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
		state_machine->RegisterState<PlayerDamageState>();
		state_machine->RegisterState<PlayerDeadState>();
		state_machine->RegisterState<PlayerDeadIdleState>();

		state_machine->SetDefaultState(PlayerIdleState::STATE_NAME);	// �f�t�H���g�X�e�[�g�̐ݒ�
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
		param.max_accelerationXZ = 100.0f;
		param.is_stage_raycas = true;
		auto movement = player->AddComponent<MovementComponent>(param);
	}
	// �v���C���[�R���|�[�l���g�쐬
	{
		PlayerComponent::PlayerParam param{};
		param.move_speed = 15.0f;
		player_component = player->AddComponent<PlayerComponent>(param);
	}
	// �V�F�[�_�[�ݒ�
	if (ModelShader::Instance model_shader = ModelShader::GetInstance(); model_shader.Get())
	{
			auto shader_component = player->AddComponent<ModelShaderComponent>(model_shader.Get());
	}
	// �d��
	{
		player->AddComponent<GravityComponent>(GravityComponent::GravityParam());
	}
	// �L�����N�^�[�X�e�[�^�X
	{
		CharacterComponent::CharacterParam param{};
		param.max_hp = 100;
		param.hp = 100;
		player->AddComponent<CharacterComponent>(param);
	}


	// �q�I�u�W�F�N�g
	{
		// �v���C���[�J����
		{
			std::shared_ptr<Object> player_camera_object = player->CreateChildObject();
			player_camera_object->SetName(PLAYER_CAMERA_NAME.GetString().c_str());
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
				param.max_angleX = DirectX::XMConvertToRadians(70.0f);
				param.min_angleX = DirectX::XMConvertToRadians(15.0f);
				param.roll_speedX = DirectX::XMConvertToRadians(60.0f);
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
			player_attack_object->SetName(ATTACK_OBJECT_NAME.GetString().c_str());
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
					child_collision->AddCollisionComponent(player_component);
				}
			}
		}
		// ��]�U���p�����蔻��I�u�W�F�N�g
		{
			std::shared_ptr<Object> player_attack_object = player->CreateChildObject();
			player_attack_object->SetName(SPIN_ATTACK_OBJECT_NAME.GetString().c_str());
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
					child_collision->AddCollisionComponent(player_component);
				}
			}
		}
	}

	return player;
}