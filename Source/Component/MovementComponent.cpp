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

	float lengthXZ_sq = this->acceleration.LengthXZSq();
	float max_accelerationXZ_sq = this->max_accelerationXZ * max_accelerationXZ;
	
    // 速度計算

	// 加算量
	if (max_accelerationXZ_sq < lengthXZ_sq)
	{
		float lengthXZ = sqrt(lengthXZ_sq);
		this->velocity = this->acceleration / lengthXZ * this->max_accelerationXZ * elapsed_time;
	}
	else
	{
		this->velocity = this->acceleration * elapsed_time;
	}
	this->velocity.SetY(this->acceleration.GetY() * elapsed_time);

	// ステージとのレイキャスト
    if(is_stage_raycas)
    {
		auto stage_object = GameObject::Instance()->GetGameObject(GameObject::OBJECT_TYPE::STAGE);
		auto stage_model = stage_object->EnsureComponentValid<ModelComponent>(this->stage_model_Wptr);
		if (stage_model && stage_object)
		{
			auto gravity = owner->EnsureComponentValid<GravityComponent>(this->gravity_Wptr);
			// 地面方向にレイキャストを行う
			if (gravity)
			{
				// Y軸の下方向に向けてレイキャストを行う
				{
					// 現在の位置
					const DirectX::XMFLOAT3 current_pos = transform->GetPosition();

					// 垂直方向の移動量
					float my = this->velocity.GetY();

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
			}

			// 前方方向にレイキャストを行う
			{
				// 現在の位置
				const DirectX::XMFLOAT3 current_pos = transform->GetPosition();

				float velocity_lengthXZ = this->velocity.LengthXZ();
				if (velocity_lengthXZ > 0.0f)
				{
					// 水平方向の移動量
					float mx = this->velocity.GetX();
					float mz = this->velocity.GetZ();

					// レイの開始位置と終点位置
					MYVECTOR3 start = { current_pos.x,current_pos.y + step_offset,current_pos.z };
					MYVECTOR3 end = { current_pos.x + mx,current_pos.y + step_offset,current_pos.z + mz };

#ifdef _DEBUG
					// デバッグプリミティブ表示
					{
						DebugRenderer* debug_render = DebugManager::Instance()->GetDebugRenderer();
						debug_render->DrawSphere(start.GetFlaot3(), 0.05f, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
						debug_render->DrawSphere(end.GetFlaot3(), 0.05f, DirectX::XMFLOAT4(0.0f, 0.0f, 0.5f, 1.0f));
					}
#endif // _DEBUG

					// レイキャスト壁判定
					HitResult hit;
					if (Collision::IntersectRayVsModel(start, end, stage_model.get(), hit))
					{
						// 壁からレイの終点までのベクトル
						MYVECTOR3 vecSE = end - hit.position;

						// 壁の法線
						MYVECTOR3& normal = hit.normal;

						// 入射ベクトルを法線に射影
						MYVECTOR3 dot = (vecSE.Negate()).Dot(normal) * 1.1f;

						// 補正位置の計算
						MYVECTOR3 correction_positon = normal * dot + end;

						// 壁ずり方向へのレイキャスト
						HitResult hit2;
						if (!Collision::IntersectRayVsModel(start, correction_positon, stage_model.get(), hit2))
						{
							MYVECTOR3 positon = correction_positon;
							positon.SetY(current_pos.y);
							transform->SetPosition(positon);
						}
						else
						{
							MYVECTOR3 positon = hit2.position;
							positon.SetY(current_pos.y);
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
    }
	else
    {
        transform->AddPosition(this->velocity.GetFlaot3());
    }

	// 加速度を初期化
	this->acceleration = {};
}

bool MovementComponent::IsMoveXZAxis()
{
    return (this->velocity.GetX() != 0.0f || this->velocity.GetZ() != 0.0f);
}

#ifdef _DEBUG

void MovementComponent::DrawDebugGUI()
{
    //ImGui::InputFloat3("Additional Velocity", &this->acceleration.x);
    ImGui::InputFloat("Max AccelerationXZ", &this->max_accelerationXZ);
    //ImGui::InputFloat3("Velocity", &this->velocity.x);
	ImGui::Checkbox("Is Stage Raycas", &this->is_stage_raycas);
	if (this->is_stage_raycas)
	{
		ImGui::InputFloat("Step Offset", &this->step_offset);
	}
}

#endif _DEBUG