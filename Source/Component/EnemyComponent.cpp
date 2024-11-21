#include "EnemyComponent.h"
#include "System/MyMath/MyMathf.h"
#include "System/MyMath/MYVECTOR3.h"
#include "Object/Object.h"
#ifdef _DEBUG
#include <imgui.h>
#include "Debug/DebugManager.h"
#endif // DEBUG
#include "../Hlsl/ParticleDisp.h"
#include "ConstantManager.h"
#include "Shader\ParticleSystem.h"
#include "Camera\CameraManager.h"

#include "Component/TransformComponent.h"
#include "Component/MovementComponent.h"
#include "Component/CircleCollisionComponent.h"
#include "Component/CharacterComponent.h"
#include "Component/InstancedModelWithAnimationComponent.h"
#include "Component\CameraComponent.h"

void EnemyComponent::Start()
{
}

void EnemyComponent::End()
{
}

void EnemyComponent::Update(float elapsed_time)
{
	const auto& owner = GetOwner();
	if (!owner) return;
	const auto& model_component = owner->EnsureComponentValid(this->model_Wptr);
	if (!model_component) return;
	const auto& character = owner->EnsureComponentValid<CharacterComponent>(this->character_Wptr);

	// ���S����
	if (character && !character->IsAlive() && !this->param.pending_removal_flag)
	{
		// �폜�҂��t���O�𗧂Ă�
		this->param.pending_removal_flag = true;

		// ���S��Ԃ֑J��
		this->param.state = STATE::DETH;

		// ���S��Ԃւ̏���
		{
			model_component->PlayAnimation(EnemyCT::ANIMATION::DIE, false);
		}
	}

	// �X�e�[�g�X�V
	switch (this->param.state)
	{
	case EnemyComponent::STATE::IDLE:
	{
		if (this->param.idle_timer > 0.0f)
		{
			// �ҋ@�^�C�}�[�X�V
			this->param.idle_timer -= elapsed_time;
		}
		else
		{
			// �ҋ@���Ԃ��I��������ړ���ԂɑJ��
			this->param.state = STATE::MOVE;

			// �ړ���Ԃւ̏���
			{
				model_component->PlayAnimation(EnemyCT::MOVE_FWD, true);
				SetRandomTargetPosition();
				return;
			}
		}
		break;
	}
	case EnemyComponent::STATE::MOVE:
	{
		// �ړ�����
		{
			auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr);
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

		if (IsAtTarget())
		{
			// �ړI�n�ɓ���������ҋ@��ԂɑJ��
			this->param.state = STATE::IDLE;

			// �ҋ@��Ԃւ̏���
			{
				model_component->PlayAnimation(EnemyCT::IDLE_BATTLE, true);
				SetRandomIdleTime();
				return;
			}
		}

		break;
	}
	case EnemyComponent::STATE::DAMAGE:
	{
		if (!model_component->IsPlayAnime())
		{
			// �A�j���[�V�������I���ړ���ԂɑJ��
			this->param.state = STATE::MOVE;

			// �ړ���Ԃւ̏���
			{
				model_component->PlayAnimation(EnemyCT::MOVE_FWD, true);
				SetRandomTargetPosition();
				return;
			}
		}

		break;
	}
	case EnemyComponent::STATE::DETH:
	{
		if (model_component->IsPlayAnime()) break;	// �A�j���[�V�����Đ����Ȃ�break

		if (this->param.remove_timer > 0.0f)
		{
			// �폜�^�C�}�[�X�V
			this->param.remove_timer -= elapsed_time;
		}
		else
		{
			owner->SetIsRemove(true);
		}
		break;
	}
	default:break;
	}
}

void EnemyComponent::Move(float vx, float vz, float speed)
{
	auto owner = GetOwner();

	if (auto movement = owner->EnsureComponentValid<MovementComponent>(movement_Wptr))
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

	// �_���[�W�X�e�[�g�ɑJ�ڂ�����
	SetDamageState();

	// �a��effect�Đ�
	{
		MYVECTOR3 Pos{};    // �����ʒu
		DirectX::XMFLOAT3 pos{};    // �����ʒu
		
		// �����ʒu�v�Z
		{
			// �G�l�~�[�̈ʒu��ݒ�
			if (const auto& owner = GetOwner())
			{
				// �g�����X�t�H�[���擾
				if (const auto& child_transform = owner->EnsureComponentValid(this->child_transform_Wptr))
				{
					// �����ʒu��ݒ�
					Pos = child_transform->GetWorldPosition();
				}
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
			const float forward_offset = -2.0f;
			const float up_offset = 0.5f;
			Pos += (Forward * forward_offset) + (Up * up_offset);
			Pos.GetFlaot3(pos);
		}

		// �G�t�F�N�g�Đ�
		if (ParticleSystem::Instance particle_system = ParticleSystem::GetInstance(); particle_system.Get())
		{
			particle_system->PlayEffect(
				EFFECT_SLASH,
				pos,
				45.0f,
				DirectX::XMFLOAT3(1.0f, 0.5f, 1.0f)
			);
		}
	}
}

void EnemyComponent::SetRandomTargetPosition()
{
	float theta = MyMathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = MyMathf::RandomRange(0.0f, this->param.territory_range);
	this->param.target_position.x =  + sinf(theta) * range;
	this->param.target_position.y = 0.0f;
	this->param.target_position.z =  + cosf(theta) * range;
}

void EnemyComponent::SetRandomIdleTime()
{
	this->param.idle_timer = MyMathf::RandomRange(this->param.min_idle_time, this->param.max_idle_time);
}

bool EnemyComponent::IsAtTarget()
{
	auto owner = GetOwner();
	if (!owner) return false;
	auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr);
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

void EnemyComponent::SetDamageState()
{
	const auto& owner = GetOwner();
	if (!owner) return;
	const auto& model_component = owner->EnsureComponentValid(this->model_Wptr);
	if (!model_component) return;

	this->param.state = STATE::DAMAGE;
	model_component->PlayAnimation(EnemyCT::ANIMATION::TAUNTING, false);
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
	ImGui::Checkbox("Move Validity Flag", &this->param.move_validity_flag);
	ImGui::Checkbox("Pending Removal Flag", &this->param.pending_removal_flag);
}

void EnemyComponent::DrawDebugPrimitive()
{
	DebugManager::Instance debug_manager = DebugManager::GetInstance();
	if (!debug_manager.Get()) return;
	auto debug_render = debug_manager->GetDebugPrimitiveRenderer();
	if (debug_render && !this->IsAtTarget())
		debug_render->DrawSphere(this->param.target_position, this->param.radius, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
}

#endif // _DEBUG