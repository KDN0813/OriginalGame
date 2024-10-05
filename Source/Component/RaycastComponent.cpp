#include "RaycastComponent.h"
#include "Collision/Collision.h"
#include "Object/Object.h"
#include "Object/GameObject.h"
#ifdef _DEBUG
#include <imgui.h>
#include "Debug/DebugManager.h"

#endif // _DEBUG

#include "Component/GravityComponent.h"
#include "Component/TransformComponent.h"
#include "Component/MovementComponent.h"
#include "Component/ModelComponent.h"

void StageRaycastComponent::Update(float elapsed_time)
{
	auto owner = GetOwner();
	if (!owner) return;
	auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr);
	if (!transform) return;
	auto gravity = owner->EnsureComponentValid<GravityComponent>(this->gravity_Wptr);
	if (!gravity) return;
	auto movement = owner->EnsureComponentValid<MovementComponent>(this->movement_Wptr);
	if (!movement) return;

	auto stage_object = GameObject::Instance()->GetGameObject(GameObject::OBJECT_TYPE::STAGE);
	if (!stage_object) return;
	auto stage_model = stage_object->EnsureComponentValid<ModelComponent>(this->stage_model_Wptr);

	// Y軸の下方向に向けてレイキャストを行う
	{
		// 現在の位置
		const DirectX::XMFLOAT3 current_pos = transform->GetPosition();

		// 垂直方向の移動量
		float my = gravity->GetGravity() * elapsed_time;

		if (my < 0.0f)
		{
			// レイの開始位置と終点位置
			DirectX::XMFLOAT3 start = { current_pos.x,current_pos.y + step_offset,current_pos.z };
			DirectX::XMFLOAT3 end = { current_pos.x,current_pos.y + my,current_pos.z };

#ifdef _DEBUG	// デバッグプリミティブ表示
			{
				DebugRenderer* debug_render = DebugManager::Instance()->GetDebugRenderer();
				debug_render->DrawSphere(start, 0.05f, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
				debug_render->DrawSphere(end, 0.05f, DirectX::XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f));
			}
#endif // _DEBUG	デバッグプリミティブ表示

			// レイキャストによる地面判定
			HitResult hit;
			if (Collision::IntersectRayVsModel(start, end, stage_model.get(), hit))
			{
				transform->SetPosition(hit.position);
				gravity->SetIsGrounded(true);
			}
			else
			{
				DirectX::XMFLOAT3 position = current_pos;
				position.y += my;
				transform->SetPosition(position);

				gravity->SetIsGrounded(false);
			}
		}
	}

	// 前方方向にレイキャストを行う
	{
		// 現在の位置
		const DirectX::XMFLOAT3 current_pos = transform->GetPosition();

		const DirectX::XMFLOAT3 velocity = movement->GetVelocity();
		const float speed = movement->GetSpeed();

		float velocity_lengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
		if (velocity_lengthXZ > 0.0f)
		{
			// 水平方向の移動量
			float mx = velocity.x * elapsed_time * speed;
			float mz = velocity.z * elapsed_time * speed;

			// レイの開始位置と終点位置
			DirectX::XMFLOAT3 start = { current_pos.x,current_pos.y + step_offset,current_pos.z };
			DirectX::XMFLOAT3 end = { current_pos.x + mx,current_pos.y + step_offset,current_pos.z + mz };

#ifdef _DEBUG
			// デバッグプリミティブ表示
			{
				DebugRenderer* debug_render = DebugManager::Instance()->GetDebugRenderer();
				debug_render->DrawSphere(start, 0.05f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
				debug_render->DrawSphere(end, 0.05f, DirectX::XMFLOAT4(0.0f, 0.0f, 0.5f, 1.0f));
			}
#endif // _DEBUG

			// レイキャスト壁判定
			HitResult hit;
			if (Collision::IntersectRayVsModel(start, end, stage_model.get(), hit))
			{
				// 壁からレイの終点までのベクトル
				DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
				DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
				DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

				// 壁の法線
				DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

				// 入射ベクトルを法線に射影
				DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);
				Dot = DirectX::XMVectorScale(Dot, 1.1f);

				// 補正位置の計算
				DirectX::XMVECTOR CorrectionPositon = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
				DirectX::XMFLOAT3 correction_positon;
				DirectX::XMStoreFloat3(&correction_positon, CorrectionPositon);

				// 壁ずり方向へのレイキャスト
				HitResult hit2;
				if (!Collision::IntersectRayVsModel(start, correction_positon, stage_model.get(), hit2))
				{
					DirectX::XMFLOAT3 positon = current_pos;
					positon.x = correction_positon.x;
					positon.z = correction_positon.z;
					transform->SetPosition(positon);
				}
				else
				{
					DirectX::XMFLOAT3 positon = current_pos;
					positon.x = hit2.position.x;
					positon.z = hit2.position.z;
					transform->SetPosition(positon);
				}
			}
			else
			{
				DirectX::XMFLOAT3 positon = current_pos;
				positon.x += mx;
				positon.z += mz;
				transform->SetPosition(positon);
			}
		}
	}
}

#ifdef _DEBUG

void StageRaycastComponent::DrawDebugGUI()
{
	ImGui::InputFloat("Step Offset", &this->step_offset);
}

void StageRaycastComponent::DrawDebugPrimitive()
{
}

#endif // DEBUG