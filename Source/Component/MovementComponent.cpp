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

MovementComponent::MovementComponent()
{
#ifdef _DEBUG
	SetIsDebugPrimitive(false);

#endif // _DEBUG
}

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
		Velocity.GetFlaot3(this->velocity);
	}
	else
	{
		MYVECTOR3 Velocity = this->acceleration;
		Velocity *= elapsed_time;
		Velocity.GetFlaot3(this->velocity);
	}
	this->velocity.y = this->acceleration.y * elapsed_time;

	RaycasVsStage(owner, transform);

	FaceMovementDirection(elapsed_time);

	// 加速度を初期化
	this->acceleration = {};
}

void MovementComponent::FaceMovementDirection(float elapsed_time)
{
	float vx = this->velocity.x;
	float vz = this->velocity.z;
	float speed = this->turn_speed;

	// 進行ベクトルが0以下なら処理しない
	float length = sqrtf(vx * vx + vz * vz);
	if (length < 0.001) return;

	auto owner = GetOwner();
	if (auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr))
	{
		speed *= elapsed_time;

		// 単位ベクトル化
		vx /= length;
		vz /= length;

		// 自身の回転値から前方向を求める
		DirectX::XMFLOAT3 angle = transform->GetAngle();
		float frontX = sinf(angle.y);
		float frontZ = cosf(angle.y);

		// 回転角を求めるため、2つの単位ベクトルの内積を計算する
		float dot = (frontX * vx + frontZ * vz);

		// 内積値は-1.0~1.0で表現されており、2つの単位ベクトルの角度が
		// 小さいほど、1.0に近づくという性質を利用して回転速度を調整する
		float rot = 1.0f - dot;
		if (rot > speed) rot = speed;

		// 左右判定を行うために2つの単位ベクトルの外積を計算する
		float cross = (frontZ * vx) - (frontX * vz);

		// 2Dの外積値が正の場合か負の場合によって左右判定が行える
		//  左右判定を行うことによって左右回転を選択する
		if (cross < 0.0f)
		{
			angle.y -= rot;
		}
		else
		{
			angle.y += rot;
		}

		transform->SetAngle(angle);
	}
}

bool MovementComponent::IsMoveXZAxis() const
{
    return (this->velocity.x != 0.0f || this->velocity.z != 0.0f);
}

void MovementComponent::AddAcceleration(MYVECTOR3 Add_acc)
{
	MYVECTOR3 Acceleration = this->acceleration;
	Acceleration += Add_acc;
	Acceleration.GetFlaot3(this->acceleration);
}

void MovementComponent::AddAccelerationXZ(float x, float z)
{
	MYVECTOR3 Acceleration = this->acceleration;
	Acceleration.AddXZ(x,z);
	Acceleration.GetFlaot3(this->acceleration);
}

void MovementComponent::AddAccelerationX(float x)
{
	MYVECTOR3 Acceleration = this->acceleration;
	Acceleration.AddX(x);
	Acceleration.GetFlaot3(this->acceleration);
}

void MovementComponent::AddAccelerationY(float y)
{
	MYVECTOR3 Acceleration = this->acceleration;
	Acceleration.AddY(y);
	Acceleration.GetFlaot3(this->acceleration);
}

void MovementComponent::AddAccelerationZ(float z)
{
	MYVECTOR3 Acceleration = this->acceleration;
	Acceleration.AddZ(z);
	Acceleration.GetFlaot3(this->acceleration);
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
			const MYVECTOR3 Current_pos = transform->GetWorldPosition();

			// 垂直方向の移動量
			float my = this->velocity.y;

			if (my < 0.0f)
			{
				// レイの開始位置と終点位置
				MYVECTOR3 Start = Current_pos + MYVECTOR3(0.0f, step_offset, 0.0f);
				MYVECTOR3 End = Current_pos + MYVECTOR3(0.0f, my, 0.0f);

#ifdef _DEBUG	// デバッグプリミティブ表示用変数の更新
				{
					Start.GetFlaot3(this->rayY_start_pos);
					End.GetFlaot3(this->rayY_end_pos);
				}
#endif // _DEBUG	デバッグプリミティブ表示

				// レイキャストによる地面判定
				HitResult hit;
				if (Collision::IntersectRayVsModel(Start, End, stage_model.get(), hit))
				{
					transform->SetPosition(hit.position);
					gravity->SetIsGrounded(true);
				}
				else
				{
					DirectX::XMFLOAT3 position{};
					Current_pos.GetFlaot3(position);
					position.y += my;
					transform->SetPosition(position);

					gravity->SetIsGrounded(false);
				}
			}
		}
	}

	// 前方方向にレイキャストを行う
	{
		// 現在の位置
		const DirectX::XMFLOAT3& current_pos = transform->GetWorldPosition();
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
			// デバッグプリミティブ表示用変数の更新
			{
				Start.GetFlaot3(this->rayXZ_start_pos);
				End.GetFlaot3(this->rayXZ_end_pos);
			}
#endif // _DEBUG

			// レイキャスト壁判定
			HitResult hit;
			if (Collision::IntersectRayVsModel(Start, End, stage_model.get(), hit))
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
				if (!Collision::IntersectRayVsModel(Start, Correction_positon, stage_model.get(), hit2))
				{
					DirectX::XMFLOAT3 pos = {};
					Correction_positon.GetFlaot3(pos);
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
				MYVECTOR3 Positon = Current_pos;
				Positon.AddXZ(mx, mz);
				DirectX::XMFLOAT3 position{};
				Positon.GetFlaot3(position);
				transform->SetPosition(position);
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
	ImGui::InputFloat("Turn Speed", &this->turn_speed);
	ImGui::Checkbox("Is Stage Raycas", &this->is_stage_raycas);
	if (this->is_stage_raycas)
	{
		ImGui::InputFloat("Step Offset", &this->step_offset);
	}
}

void MovementComponent::DrawDebugPrimitive()
{
	DebugPrimitiveRenderer* debug_render = DebugManager::Instance()->GetDebugRenderer();
	debug_render->DrawSphere(this->rayY_start_pos, 0.05f, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
	debug_render->DrawSphere(this->rayY_end_pos, 0.05f, DirectX::XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f));

	debug_render->DrawSphere(this->rayXZ_start_pos, 0.05f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
	debug_render->DrawSphere(this->rayXZ_end_pos, 0.05f, DirectX::XMFLOAT4(0.0f, 0.0f, 0.5f, 1.0f));
}

#endif _DEBUG