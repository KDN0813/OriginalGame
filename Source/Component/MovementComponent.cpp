#include <imgui.h>
#include "MovementComponent.h"
#include "Object/Object.h"
#include "Object/GameObject.h"
#include "Collision/Collision.h"
#include "GridObjectManager\GridObjectManager.h"

#ifdef _DEBUG
#include "Debug/DebugManager.h"
#include "Input\Input.h"
#include "Component/PlayerComponent.h"
#endif // _DEBUG

#include "Component/TransformComponent.h"
#include "Component/GravityComponent.h"

MovementComponent::MovementComponent(MovementParam param)
	:param(param),default_param(param)
{
#ifdef _DEBUG
	SetIsDebugPrimitive(false);
	this->rayY_start_pos = SphereParam(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), 0.05f);
	this->rayY_end_pos = SphereParam(DirectX::XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f), 0.05f);
	
	this->rayXZ_start_pos = SphereParam(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), 0.05f);
	this->rayXZ_end_pos = SphereParam(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.5f), 0.05f);
#endif // _DEBUG
}

void MovementComponent::Update(float elapsed_time)
{
    auto owner = GetOwner();
	if (!owner) return;
	auto transform = owner->GetComponent<Transform3DComponent>(this->transform_Wptr);
	if (!transform) return;

	MYVECTOR3 Acceleration = this->param.acceleration;
	float lengthXZ_sq = Acceleration.LengthXZSq();
	float max_accelerationXZ_sq = this->param.max_accelerationXZ * this->param.max_accelerationXZ;
#ifdef _DEBUG
	if (Input::Instance input = Input::GetInstance(); input.Get())
	{
		if (input->GetGamePad().GetTriggerL())
		{
			max_accelerationXZ_sq = PlayerComponent::debug_move_speed * PlayerComponent::debug_move_speed;
		}
	}
#endif // _DEBUG


	// 速度計算
	if (max_accelerationXZ_sq < lengthXZ_sq)
	{
		float lengthXZ = static_cast<float>(sqrt(lengthXZ_sq));
		MYVECTOR3 Velocity = this->param.acceleration;
		Velocity = Velocity / lengthXZ * this->param.max_accelerationXZ * elapsed_time;
		Velocity.GetFlaot3(this->param.velocity);
	}
	else
	{
		MYVECTOR3 Velocity = this->param.acceleration;
		Velocity *= elapsed_time;
		Velocity.GetFlaot3(this->param.velocity);
	}
	this->param.velocity.y = this->param.acceleration.y * elapsed_time;

	FaceMovementDirection(elapsed_time);

	ResolveGridCellCollision(owner, transform,elapsed_time);

	RaycasVsStage(owner, transform);

	// 移動後にもう一度登録する
	GridObjectManager::Instance grid_object_manager = GridObjectManager::GetInstance();
	grid_object_manager->RegisterObject(owner, transform->GetWorldPosition());

	// 加速度を初期化
	this->param.acceleration = {};
}

void MovementComponent::FaceMovementDirection(float elapsed_time)
{
	float vx = this->param.velocity.x;
	float vz = this->param.velocity.z;
	float speed = this->param.turn_speed;

	// 進行ベクトルが0以下なら処理しない
	float length = sqrtf(vx * vx + vz * vz);
	if (length < 0.001) return;

	auto owner = GetOwner();
	if (auto transform = owner->GetComponent<Transform3DComponent>(this->transform_Wptr))
	{
		speed *= elapsed_time;

		// 単位ベクトル化
		vx /= length;
		vz /= length;

		// 自身の回転値から前方向を求める
		DirectX::XMFLOAT3 angle = transform->GetLocalAngle();
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

		transform->SetLocalAngle(angle);
	}
}

bool MovementComponent::IsMoveXZAxis() const
{
    return (this->param.velocity.x != 0.0f || this->param.velocity.z != 0.0f);
}

void MovementComponent::AddAcceleration(MYVECTOR3 Add_acc)
{
	MYVECTOR3 Acceleration = this->param.acceleration;
	Acceleration += Add_acc;
	Acceleration.GetFlaot3(this->param.acceleration);
}

void MovementComponent::AddAccelerationXZ(float x, float z)
{
	MYVECTOR3 Acceleration = this->param.acceleration;
	Acceleration.AddXZ(x,z);
	Acceleration.GetFlaot3(this->param.acceleration);
}

void MovementComponent::AddAccelerationX(float x)
{
	MYVECTOR3 Acceleration = this->param.acceleration;
	Acceleration.AddX(x);
	Acceleration.GetFlaot3(this->param.acceleration);
}

void MovementComponent::AddAccelerationY(float y)
{
	MYVECTOR3 Acceleration = this->param.acceleration;
	Acceleration.AddY(y);
	Acceleration.GetFlaot3(this->param.acceleration);
}

void MovementComponent::AddAccelerationZ(float z)
{
	MYVECTOR3 Acceleration = this->param.acceleration;
	Acceleration.AddZ(z);
	Acceleration.GetFlaot3(this->param.acceleration);
}

void MovementComponent::RaycasVsStage(std::shared_ptr<Object> owner,std::shared_ptr<Transform3DComponent>& transform)
{
	if (!this->param.is_stage_raycas)
	{
		transform->AddLocalPosition(this->param.velocity);
		return;
	}
	GameObject::Instance game_object = GameObject::GetInstance();
	if (!game_object.Get()) return;

	
	std::shared_ptr<ModelComponent> stage_foor_model = nullptr;
	if (auto stage_foor_object = game_object->GetStageFoor())
	{
		stage_foor_model = stage_foor_object->GetComponent<ModelComponent>(this->stage_foor_model_Wptr);
	}

	std::shared_ptr<ModelComponent> stage_wall_model = nullptr;
	if (auto stage_wall_object = game_object->GetStageWall())
	{
		stage_wall_model = stage_wall_object->GetComponent<ModelComponent>(this->stage_wall_model_Wptr);
	}
	
	auto gravity = owner->GetComponent<GravityComponent>(this->gravity_Wptr);
	// 地面方向にレイキャストを行う
	if (gravity && stage_foor_model)
	{
		// Y軸の下方向に向けてレイキャストを行う
		{
			// 現在の位置
			const MYVECTOR3 Current_pos = transform->GetWorldPosition();

			// 垂直方向の移動量
			float my = this->param.velocity.y;

			if (my < 0.0f)
			{
				// レイの開始位置と終点位置
				MYVECTOR3 Start = Current_pos + MYVECTOR3(0.0f, this->param.step_offset, 0.0f);
				MYVECTOR3 End = Current_pos + MYVECTOR3(0.0f, my, 0.0f);

#ifdef _DEBUG	// デバッグプリミティブ表示用変数の更新
				{
					DirectX::XMFLOAT3 start_pos{};
					Start.GetFlaot3(start_pos);
					this->rayY_start_pos.SetCenter(start_pos);
					DirectX::XMFLOAT3 end_pos{};
					End.GetFlaot3(end_pos);
					this->rayY_end_pos.SetCenter(end_pos);
				}
#endif // _DEBUG	デバッグプリミティブ表示

				// レイキャストによる地面判定
				RayHitResult hit;
				if (Collision::IntersectRayVsModel(Start, End, stage_foor_model.get(), hit))
				{
					transform->SetLocalPosition(hit.position);
					gravity->SetIsGrounded(true);
				}
				else
				{
					DirectX::XMFLOAT3 position{};
					Current_pos.GetFlaot3(position);
					position.y += my;
					transform->SetLocalPosition(position);

					gravity->SetIsGrounded(false);
				}
			}
		}
	}

	// 前方方向にレイキャストを行う
	if(stage_wall_model)
	{
		// 現在の位置
		const DirectX::XMFLOAT3& current_pos = transform->GetWorldPosition();
		const MYVECTOR3 Current_pos = current_pos;

		float velocity_lengthXZ = MYVECTOR3(this->param.velocity).LengthXZ();
		
#ifdef _DEBUG
		// デバッグプリミティブ表示用変数の更新
		{
			MYVECTOR3 Start = Current_pos + MYVECTOR3(0.0f, this->param.step_offset, 0.0f);

			DirectX::XMFLOAT3 start_pos{};
			Start.GetFlaot3(start_pos);
			this->rayXZ_start_pos.SetCenter(start_pos);
			this->rayXZ_end_pos.SetCenter(start_pos);		// 移動してない場合も終点を変更するためstart_posを代入している
		}
#endif // _DEBUG

		if (velocity_lengthXZ > 0.0f)
		{
			// 水平方向の移動量
			float mx = this->param.velocity.x;
			float mz = this->param.velocity.z;
			const float adjustment_rate = 0.01f;

			// レイの開始位置と終点位置
			MYVECTOR3 Start = Current_pos + MYVECTOR3(-mx * adjustment_rate, this->param.step_offset, -mz * adjustment_rate);
			MYVECTOR3 End = Current_pos + MYVECTOR3(mx, this->param.step_offset, mz);

#ifdef _DEBUG
			// デバッグプリミティブ表示用変数の更新
			{
				DirectX::XMFLOAT3 end_pos{};
				End.GetFlaot3(end_pos);
				this->rayXZ_end_pos.SetCenter(end_pos);
			}
#endif // _DEBUG

			// レイキャスト壁判定
			RayHitResult hit;
			if (Collision::IntersectRayVsModel(Start, End, stage_wall_model.get(), hit))
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
				RayHitResult hit2;
				if (!Collision::IntersectRayVsModel(S, Correction_positon, stage_wall_model.get(), hit2))
				{
					DirectX::XMFLOAT3 pos = {};
					Correction_positon.GetFlaot3(pos);
					pos.y = current_pos.y;
					transform->SetLocalPosition(pos);
				}
				else
				{
					DirectX::XMFLOAT3 pos = { hit2.position.x, current_pos.y, hit2.position.z };
					transform->SetLocalPosition(pos);
				}
			}
			else
			{
				MYVECTOR3 Positon = Current_pos;
				Positon.AddXZ(mx, mz);
				DirectX::XMFLOAT3 position{};
				Positon.GetFlaot3(position);
				transform->SetLocalPosition(position);
			}
		}
	}
}

void MovementComponent::ResolveGridCellCollision(std::shared_ptr<Object> owner, std::shared_ptr<Transform3DComponent>& transform,float elapsed_time)
{
	MYVECTOR3 new_position = transform->GetWorldPosition(); // 移動後の位置
	MYVECTOR3 current_velocity = this->param.velocity;
	new_position += current_velocity;

	DirectX::XMFLOAT3 position_float3{};
	new_position.GetFlaot3(position_float3);

	GridObjectManager::Instance grid_object_manager = GridObjectManager::GetInstance();
	if (!grid_object_manager->RegisterObject(owner, position_float3))
	{
		// 既に登録されている場合
		this->param.velocity = {};
	}
}

#ifdef _DEBUG

void MovementComponent::DrawDebugGUI()
{
	ImGui::InputFloat3("Additional Velocity", &this->param.acceleration.x);
    ImGui::InputFloat("Max AccelerationXZ", &this->param.max_accelerationXZ);
	ImGui::InputFloat3("Velocity", &this->param.velocity.x);
	ImGui::InputFloat("Turn Speed", &this->param.turn_speed);
	ImGui::Checkbox("Is Stage Raycas", &this->param.is_stage_raycas);
	if (this->param.is_stage_raycas)
	{
		ImGui::InputFloat("Step Offset", &this->param.step_offset);
	}

	auto owner = GetOwner();
	if (!owner) return;
	auto transform = owner->GetComponent<Transform3DComponent>(this->transform_Wptr);
	if (!transform) return;

	GridObjectManager::Instance grid_object_manager = GridObjectManager::GetInstance();
	int cell_index = grid_object_manager->GetCellIndex(transform->GetWorldPosition());
	DirectX::XMFLOAT3 cell_center = grid_object_manager->GetCellCenter(cell_index);

	ImGui::InputInt("cell_index", &cell_index);
	ImGui::InputFloat3("cell_center", &cell_center.x);
}

void MovementComponent::DrawDebugPrimitive()
{
	DebugManager::Instance debug_manager = DebugManager::GetInstance();

	DebugPrimitiveRenderer* debug_render = debug_manager->GetDebugPrimitiveRenderer();
	debug_render->DrawSphere(this->rayY_start_pos);
	debug_render->DrawSphere(this->rayY_end_pos);

	debug_render->DrawSphere(this->rayXZ_start_pos);
	debug_render->DrawSphere(this->rayXZ_end_pos);
}

void MovementComponent::DrawDebugPrimitiveGUI()
{
	this->rayY_start_pos.DrawDebugGUI("RayY Start");
	this->rayY_end_pos.DrawDebugGUI("RayY End");
	this->rayXZ_start_pos.DrawDebugGUI("RayXZ Start");
	this->rayXZ_end_pos.DrawDebugGUI("RayXZ End");
}

#endif _DEBUG