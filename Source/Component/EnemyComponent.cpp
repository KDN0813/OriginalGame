#include "EnemyComponent.h"
#include "System/MyMath/MyMathf.h"
#include "System/MyMath/MYVECTOR3.h"
#include "Object/Object.h"
#ifdef _DEBUG
#include <imgui.h>
#include "Debug/DebugManager.h"
#endif // DEBUG
#include "../Hlsl/ParticleDisp.h"
#include "Shader\ParticleSystem.h"
#include "Camera\CameraManager.h"
#include "System\GameData.h"
#include "Object\GameObject.h"
#include "Object\Constant\EnemyConstant.h"

#include "Component/TransformComponent.h"
#include "Component/MovementComponent.h"
#include "Component/CircleCollisionComponent.h"
#include "Component/CharacterComponent.h"
#include "Component/InstancedModelWithAnimationComponent.h"
#include "Component\CameraComponent.h"

void EnemyComponent::Start()
{
	auto owner = GetOwner();
	if (!owner) return;
	auto transform = owner->GetComponent<Transform3DComponent>(this->transform_Wptr);
	if (!transform) return;
}

void EnemyComponent::End()
{
}

void EnemyComponent::Update(float elapsed_time)
{
	const auto& owner = GetOwner();
	if (!owner) return;
	const auto& model_component = owner->GetComponent(this->model_Wptr);
	if (!model_component) return;
	const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);

	auto transform = owner->GetComponent<Transform3DComponent>(this->transform_Wptr);
	// �ړ�����
	if (transform && this->param.move_validity_flag)
	{
		// �ړI�n�_�܂ł�XZ���ʂł̋�������
		MYVECTOR3 Position = transform->GetWorldPosition();
		MYVECTOR3 Target_position = this->param.target_position;
		float distSq = (Target_position.GetMyVectorXZ() - Position.GetMyVectorXZ()).LengthSq();

		if (!this->IsAtTarget(distSq))
		{
			// �ړI�n�_�ֈړ�
			MoveToTarget(elapsed_time, transform, this->param.speed_rate);
		}
	}
}

void EnemyComponent::Move(float vx, float vz, float speed)
{
	auto owner = GetOwner();

	if (auto movement = owner->GetComponent<MovementComponent>(movement_Wptr))
	{
		movement->AddAccelerationXZ(vx * speed, vz * speed);
	}
}

void EnemyComponent::MoveToTarget(float elapsed_time, std::shared_ptr<Transform3DComponent>& transform, float speed_rate)
{
	// �^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
	MYVECTOR3 Position = transform->GetWorldPosition();
	MYVECTOR3 Target_position = this->param.target_position;
	MYVECTOR3 Vec = (Target_position.GetMyVectorXZ() - Position.GetMyVectorXZ()).Normalize();
	float vx = Vec.GetX();
	float vz = Vec.GetZ();

	// �ړ�����
	Move(vx, vz, this->param.move_speed * speed_rate);
}

void EnemyComponent::OnCollision(const std::shared_ptr<Object>& hit_object)
{
	if (this->param.pending_removal_flag) return;	// �폜�҂��̏ꍇreturn

	const auto& owner = GetOwner();
	if (!owner) return;
	const auto collision = owner->GetComponent(circle_collision_Wptr);
	if (!collision) return;

	switch (collision->GetCircleHitResult().collision_role)
	{
	case COLLISION_ROLE::ATTACKER:	// �Փ˂�^�������̏���
	{
		// �q�b�g�����I�u�W�F�N�g�Ƀ_���[�W��^����
		const auto& hit_object_character = hit_object->GetComponent<CharacterComponent>();
		if (hit_object_character)
		{
			hit_object_character->ApplyDamage(this->param.damage_amount);
		}
		break;
	}
	case COLLISION_ROLE::RECEIVER:	// �Փ˂��󂯂����̏���
	{
		// �a��effect�Đ�
		{
			MYVECTOR3 Pos{};    // �����ʒu
			DirectX::XMFLOAT3 pos{};    // �����ʒu

			// �����ʒu�v�Z
			{
				// �g�����X�t�H�[���擾
				if (const auto& child_transform = owner->GetComponent(this->child_transform_Wptr))
				{
					// �����ʒu��ݒ�
					Pos = child_transform->GetWorldPosition();
				}

				// �J�����̌��擾
				MYVECTOR3 Forward = MYVECTOR3(1.0f, 0.0f, 0.0f);
				MYVECTOR3 Up = MYVECTOR3(0.0f, 1.0f, 0.0f);
				if (CameraManager::Instance camera_manager = CameraManager::GetInstance(); camera_manager.Get())
				{
					if (const auto& camera = camera_manager->GetCamera(CAMERA_TYPE::MAIN))
					{
						Forward = camera->GetForward();
						Up = camera->GetUp();
					}
				}

				// �Đ��ʒu�v�Z
				Pos += (Up * this->param.damage_effect_offset_up);
				Pos.GetFlaot3(pos);
			}

			// �G�t�F�N�g�Đ�
			if (ParticleSystem::Instance particle_system = ParticleSystem::GetInstance(); particle_system.Get())
			{
				particle_system->PlayEffect(
					EFFECT_HIT,
					pos,
					45.0f,
					DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f)
				);
			}
		}
	}
		break;
	default:
		break;
	}
}

void EnemyComponent::SetRandomTargetPosition()
{
	float theta = MyMath::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = MyMath::RandomRange(0.0f, this->param.territory_range);
	this->param.target_position.x = this->param.spawn_point.x + sinf(theta) * range;
	this->param.target_position.y = 0.0f;
	this->param.target_position.z = this->param.spawn_point.z + cosf(theta) * range;
}

void EnemyComponent::SetTargetPositionByPlayer()
{
	if (GameObject::Instance game_object = GameObject::GetInstance(); game_object.Get())
	{
		const auto& player = game_object->GetPlayer();
		if (!player) return;
		const auto& player_transform = player->GetComponent<Transform3DComponent>(this->player_transform_Wptr);
		if (!player_transform) return;
		this->param.target_position = player_transform->GetWorldPosition();
	}
}

void EnemyComponent::SetRandomIdleTime()
{
	this->param.idle_timer = MyMath::RandomRange(this->param.min_idle_time, this->param.max_idle_time);
}

bool EnemyComponent::IsAtTarget()
{
	auto owner = GetOwner();
	if (!owner) return false;
	auto transform = owner->GetComponent<Transform3DComponent>(this->transform_Wptr);
	if (!transform) return false;

	// �ړI�n�_�܂ł�XZ���ʂł̋�������
	MYVECTOR3 Position = transform->GetWorldPosition();
	MYVECTOR3 Target_position = this->param.target_position;
	float distSq = (Target_position.GetMyVectorXZ() - Position.GetMyVectorXZ()).LengthSq();

	return IsAtTarget(distSq);
}

bool EnemyComponent::IsAtTarget(float distSq)
{
	return (distSq < this->param.radius * this->param.radius);
}

bool EnemyComponent::IsPlayerInMovementArea()
{
	MYVECTOR3 SpawnPosition = this->param.spawn_point;	// �����ʒu(�ړ��͈͂̒��S)
	MYVECTOR3 Player_position{};					// �v���C���[�̈ʒu

	// �v���C���[�擾
	GameObject::Instance game_object = GameObject::GetInstance();
	if (!game_object.Get()) return false;
	const auto& player = game_object->GetPlayer();
	if (!player) return false;

	// �v���C���[�̈ʒu�ݒ�
	const auto& player_transform = player->GetComponent<Transform3DComponent>(this->player_transform_Wptr);
	if (!player_transform) return false;
	Player_position = player_transform->GetWorldPosition();
	
	// �͈͓��ɑ��݂��邩���肷��
	float distSq = (Player_position.GetMyVectorXZ() - SpawnPosition.GetMyVectorXZ()).LengthSq();
	return (distSq < this->param.territory_range * this->param.territory_range);
}

bool EnemyComponent::IsPlayerInAttacklArea()
{
	// ���g�̈ʒu�擾
	auto owner = GetOwner();
	if (!owner) return false;
	auto transform = owner->GetComponent<Transform3DComponent>(this->transform_Wptr);
	if (!transform) return false;
	MYVECTOR3 Position = transform->GetWorldPosition();

	// �v���C���[�擾
	GameObject::Instance game_object = GameObject::GetInstance();
	if (!game_object.Get()) return false;
	const auto& player = game_object->GetPlayer();
	if (!player) return false;
	MYVECTOR3 Player_position{};

	// �v���C���[�̈ʒu�ݒ�
	const auto& player_transform = player->GetComponent<Transform3DComponent>(this->player_transform_Wptr);
	if (!player_transform) return false;
	Player_position = player_transform->GetWorldPosition();

	// �͈͓��ɑ��݂��邩���肷��
	float distSq = (Player_position.GetMyVectorXZ() - Position.GetMyVectorXZ()).LengthSq();
	return (distSq < this->param.attack_range * this->param.attack_range);
}

void EnemyComponent::SetDamageState()
{
	const auto& owner = GetOwner();
	if (!owner) return;
	const auto& model_component = owner->GetComponent(this->model_Wptr);
	if (!model_component) return;

	this->param.state = STATE::DAMAGE;
	model_component->PlayAnimation(EnemyConstant::ANIMATION::TAUNTING, false);
}

#ifdef _DEBUG

void EnemyComponent::DrawDebugGUI()
{
	ImGui::InputFloat3("Target Position", &this->param.target_position.x);
	ImGui::DragFloat("Territory Range", &this->param.territory_range, 0.01f);
	ImGui::DragFloat("Radius", &this->param.radius, 0.01f);
	ImGui::DragFloat("Move Speed", &this->param.move_speed);
	ImGui::DragFloat("Speed Rate", &this->param.speed_rate);
	ImGui::InputFloat("Idle Timer", &this->param.idle_timer);
	ImGui::InputFloat("Idle Max Timer", &this->param.max_idle_time);
	ImGui::InputFloat("Idle Min Timer", &this->param.min_idle_time);
	ImGui::InputFloat("Remove Timer", &this->param.remove_timer);
	ImGui::InputFloat("Damage Effect Offset Up", &this->param.damage_effect_offset_up);
	ImGui::Checkbox("Move Validity Flag", &this->param.move_validity_flag);
	ImGui::Checkbox("Pending Removal Flag", &this->param.pending_removal_flag);
}

void EnemyComponent::DrawDebugPrimitive()
{
	DebugManager::Instance debug_manager = DebugManager::GetInstance();
	if (!debug_manager.Get()) return;
	auto debug_render = debug_manager->GetDebugPrimitiveRenderer();

	const auto& owner = GetOwner();
	if (!owner) return;
	auto transform = owner->GetComponent<Transform3DComponent>(this->transform_Wptr);
	if (!transform) return;
	MYVECTOR3 Position = transform->GetWorldPosition();

	// �ړI�n
	//if (debug_render && !this->IsAtTarget())
	//{
	//	debug_render->DrawSphere(this->param.target_position, 1.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
	//}
	
	if (debug_render)
	{
		// �ړ��͈͕`��
		//debug_render->DrawCylinder(this->param.spawn_point, this->param.territory_range, 2.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		
		if (IsPlayerInMovementArea())
		{
			// �U���͈�
			debug_render->DrawCylinder(transform->GetWorldPosition(), this->param.attack_range, 2.0f, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}
}

#endif // _DEBUG