#include <imgui.h>
#include "MovementComponent.h"
#include "Object/Object.h"
#include "Object/GameObject.h"
#include "Collision/Collision.h"
#ifdef _DEBUG
#include "Debug/DebugManager.h"

#endif // _DEBUG

#include "Component/TransformComponent.h"
#include "Component/GravityComponent.h"

void MovementComponent::Update(float elapsed_time)
{
    auto owner = GetOwner();
	if (!owner) return;
	auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr);
	if (!transform) return;

	MYVECTOR3 Acceleration = this->acceleration;
	float lengthXZ_sq = Acceleration.LengthXZSq();
	float max_accelerationXZ_sq = this->max_accelerationXZ * max_accelerationXZ;
	
	// 速度計算
	if (max_accelerationXZ_sq < lengthXZ_sq)
	{
		float lengthXZ = static_cast<float>(sqrt(lengthXZ_sq));
		MYVECTOR3 Velocity = this->acceleration;
		Velocity = Velocity / lengthXZ * this->max_accelerationXZ * elapsed_time;
		this->velocity = Velocity.GetFlaot3();
	}
	else
	{
		MYVECTOR3 Velocity = this->acceleration;
		Velocity *= elapsed_time;
		this->velocity = Velocity.GetFlaot3();
	}
	this->velocity.y = this->acceleration.y * elapsed_time;

	RaycasVsStage(owner, transform);

	// 加速度を初期化
	this->acceleration = {};
}

bool MovementComponent::IsMoveXZAxis()
{
    return (this->velocity.x != 0.0f || this->velocity.z != 0.0f);
}

void MovementComponent::AddAcceleration(MYVECTOR3 Add_acc)
{
	MYVECTOR3 Acceleration = this->acceleration;
	Acceleration += Add_acc;
	this->acceleration = Acceleration.GetFlaot3();
}

void MovementComponent::AddAccelerationXZ(float x, float z)
{
	MYVECTOR3 Acceleration = this->acceleration;
	Acceleration.AddXZ(x,z);
	this->acceleration = Acceleration.GetFlaot3();
}

void MovementComponent::AddAccelerationX(float x)
{
	MYVECTOR3 Acceleration = this->acceleration;
	Acceleration.AddX(x);
	this->acceleration = Acceleration.GetFlaot3();
}

void MovementComponent::AddAccelerationY(float y)
{
	MYVECTOR3 Acceleration = this->acceleration;
	Acceleration.AddY(y);
	this->acceleration = Acceleration.GetFlaot3();
}

void MovementComponent::AddAccelerationZ(float z)
{
	MYVECTOR3 Acceleration = this->acceleration;
	Acceleration.AddZ(z);
	this->acceleration = Acceleration.GetFlaot3();
}

void MovementComponent::RaycasVsStage(std::shared_ptr<Object> owner,std::shared_ptr<Transform3DComponent>& transform)
{
	if (!this->is_stage_raycas)
	{
		transform->AddPosition(this->velocity);
		return;
	}
	auto stage_object = GameObject::Instance()->GetGameObject(GameObject::OBJECT_TYPE::STAGE);
	if (!stage_object)
	{
		transform->AddPosition(this->velocity);
		return;
	}
	auto stage_model = stage_object->EnsureComponentValid<ModelComponent>(this->stage_model_Wptr);
	if (!stage_model)
	{
		transform->AddPosition(this->velocity);
		return;
	}

	auto gravity = owner->EnsureComponentValid<GravityComponent>(this->gravity_Wptr);
	// 地面方向にレイキャストを行う
	if (gravity)
	{
		// Y軸の下方向に向けてレイキャストを行う
		{
			// 現在の位置
			const MYVECTOR3 Current_pos = transform->GetPosition();

			// 垂直方向の移動量
			float my = this->velocity.y;

			if (my < 0.0f)
			{
				// レイの開始位置と終点位置
				MYVECTOR3 Start = Current_pos + MYVECTOR3(0.0f, step_offset, 0.0f);
				MYVECTOR3 End = Current_pos + MYVECTOR3(0.0f, my, 0.0f);

#ifdef _DEBUG	// デバッグプリミティブ表示
				{
					DebugRenderer* debug_render = DebugManager::Instance()->GetDebugRenderer();
					debug_render->DrawSphere(Start.GetFlaot3(), 0.05f, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
					debug_render->DrawSphere(End.GetFlaot3(), 0.05f, DirectX::XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f));
				}
#endif // _DEBUG	デバッグプリミティブ表示

				// レイキャストによる地面判定
				HitResult hit;
				if (Collision::IntersectRayVsModel(Start.GetFlaot3(), End.GetFlaot3(), stage_model.get(), hit))
				{
					transform->SetPosition(hit.position);
					gravity->SetIsGrounded(true);
				}
				else
				{
					MYVECTOR3 Position = Current_pos;
					Position.AddY(my);
					transform->SetPosition(Position.GetFlaot3());

					gravity->SetIsGrounded(false);
				}
			}
		}
	}

	// 前方方向にレイキャストを行う
	{
		// 現在の位置
		const DirectX::XMFLOAT3& current_pos = transform->GetPosition();
		const MYVECTOR3 Current_pos = current_pos;

		float velocity_lengthXZ = MYVECTOR3(this->velocity).LengthXZ();
		if (velocity_lengthXZ > 0.0f)
		{
			// 水平方向の移動量
			float mx = this->velocity.x;
			float mz = this->velocity.z;

			// レイの開始位置と終点位置
			MYVECTOR3 Start = Current_pos + MYVECTOR3(0.0f, step_offset, 0.0f);
			MYVECTOR3 End = Current_pos + MYVECTOR3(mx, step_offset, mz);

#ifdef _DEBUG
			// デバッグプリミティブ表示
			{
				DebugRenderer* debug_render = DebugManager::Instance()->GetDebugRenderer();
				debug_render->DrawSphere(Start.GetFlaot3(), 0.05f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
				debug_render->DrawSphere(End.GetFlaot3(), 0.05f, DirectX::XMFLOAT4(0.0f, 0.0f, 0.5f, 1.0f));
			}
#endif // _DEBUG

			// レイキャスト壁判定
			HitResult hit;
			if (Collision::IntersectRayVsModel(Start.GetFlaot3(), End.GetFlaot3(), stage_model.get(), hit))
			{
				// 壁からレイの終点までのベクトル
				MYVECTOR3 S = hit.position;
				MYVECTOR3 VecSE = End - S;

				// 壁の法線
				MYVECTOR3 Normal = hit.normal;

				// 入射ベクトルを法線に射影
				MYVECTOR3 Dot = (VecSE.Negate()).DotVec3(Normal) * 1.1f;

				// 補正位置の計算
				MYVECTOR3 Correction_positon = Normal * Dot + End;
				
				// 壁ずり方向へのレイキャスト
				HitResult hit2;
				if (!Collision::IntersectRayVsModel(Start.GetFlaot3(), Correction_positon.GetFlaot3(), stage_model.get(), hit2))
				{
					DirectX::XMFLOAT3 pos = Correction_positon.GetFlaot3();
					pos.y = current_pos.y;
					transform->SetPosition(pos);
				}
				else
				{
					DirectX::XMFLOAT3 pos = { hit2.position.x, current_pos.y, hit2.position.z };
					transform->SetPosition(pos);
				}
			}
			else
			{
				MYVECTOR3 positon = Current_pos;
				positon.AddXZ(mx, mz);
				transform->SetPosition(positon.GetFlaot3());
			}
		}
	}
}

#ifdef _DEBUG

void MovementComponent::DrawDebugGUI()
{
	ImGui::InputFloat3("Additional Velocity", &this->acceleration.x);
    ImGui::InputFloat("Max AccelerationXZ", &this->max_accelerationXZ);
	ImGui::InputFloat3("Velocity", &this->velocity.x);
	ImGui::Checkbox("Is Stage Raycas", &this->is_stage_raycas);
	if (this->is_stage_raycas)
	{
		ImGui::InputFloat("Step Offset", &this->step_offset);
	}
}

#endif _DEBUG