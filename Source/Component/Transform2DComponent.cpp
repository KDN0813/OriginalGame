#include "Transform2DComponent.h"
#include <imgui.h>
#include "Object/Object.h"
#include <cmath>

#include "Debug/DebugManager.h"

Transform2DComponent::Transform2DComponent(Transform2DParam param)
	:param(param), default_param(param)
{
#ifdef _DEBUG
	this->sphere_world_position = SphereParam(DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), 0.1f);
	SetIsDebugPrimitive(false);
#endif // _DEBUG
}

void Transform2DComponent::Update(float elapsed_time)
{
	// ローカルトランスフォームの更新
	if (this->local_dirty_flag) UpdateLocalParam();

	if (this->world_dirty_flag)
	{
		// ワールドトランスフォームの更新
		UpdateWorldParam();
	}
}

void Transform2DComponent::UpdateWorldParam()
{
	// 親の行列取得
	MYMATRIX Parent_transform = DirectX::XMMatrixIdentity();
	if (const auto& owner = GetOwner())
	{
		if (const auto& parent = owner->GetParent())
		{
			if (auto parent_transform = parent->EnsureComponentValid(this->parent_ransform_Wptr))
			{
				Parent_transform = parent_transform->GetWolrdTransform();
			}
		}
	}

	// ローカルトランスフォーム取得
	if (this->local_dirty_flag) UpdateLocalParam();	// ローカルの値が変更されているならローカルパラメータの更新する
	MYMATRIX Local_transform = this->local_transform;

	// ワールド行列作成
	MYMATRIX World_transform = Local_transform * Parent_transform;
	World_transform.GetFlaot4x4(this->world_transform);

	// ワールドパラメータの更新
	{
		// 位置の取得
		MYVECTOR2 Local_position = this->param.local_position;
		MYVECTOR2 Worldl_position = Parent_transform.Vector2TransformCoord(Local_position);
		Worldl_position.GetFlaot2(this->world_position);

		// スケールの取得
		float scaleX = std::sqrt(this->world_transform._11 * this->world_transform._11 + this->world_transform._12 * this->world_transform._12);
		float scaleY = std::sqrt(this->world_transform._21 * this->world_transform._21 + this->world_transform._22 * this->world_transform._22);
		this->world_scale = DirectX::XMFLOAT2(scaleX, scaleY);

		// 回転角度の取得
		// TODO 親子関係ができていないので修正する
		this->world_angle = std::atan2(this->world_transform._12 / scaleX, this->world_transform._11 / scaleX);
	}

	this->world_dirty_flag = false;	// フラグを解除
}

void Transform2DComponent::UpdateLocalParam()
{
	// ローカル行列作成
	MYMATRIX Local_transform;
	Local_transform.SetLocalMatrix(this->param.local_scale, this->param.local_angle, this->param.local_position);
	Local_transform.GetFlaot4x4(this->local_transform);

	this->local_dirty_flag = false;	// フラグを解除
}

const DirectX::XMFLOAT4X4& Transform2DComponent::GetWolrdTransform()
{
	if (this->world_dirty_flag) UpdateWorldParam();
	return this->world_transform;
}

const DirectX::XMFLOAT4X4& Transform2DComponent::GetLocalTransform()
{
	if (this->local_dirty_flag) UpdateLocalParam();
	return this->local_transform;
}

void Transform2DComponent::SetLocalPosition(DirectX::XMFLOAT2 pos) noexcept
{
	// ローカル座標を更新
	this->param.local_position = pos;

	// ワールドおよびローカルパラメータの更新が必要なフラグを立てる
	this->world_dirty_flag = true;
	this->local_dirty_flag = true;

	// 子オブジェクトのトランスフォームに値更新が必要なフラグを立てる
	{
		if (const auto& owner = GetOwner())
		{
			for (const auto& chilled : owner->GetChildren())
			{
				if (auto chilled_transform = chilled->GetComponent<Transform2DComponent>())
				{
					chilled_transform->SetWorldDirtyFlag();
				}
			}
		}
	}
}

DirectX::XMFLOAT2 Transform2DComponent::AddLocalPosition(DirectX::XMFLOAT2 vec)
{
	// ローカル座標の更新
	MYVECTOR2 Pos = this->param.local_position;
	Pos += vec;
	Pos.GetFlaot2(this->param.local_position);

	// ワールドおよびローカルパラメータの更新が必要なフラグを立てる
	this->world_dirty_flag = true;
	this->local_dirty_flag = true;

	// 子オブジェクトのトランスフォームに値更新が必要なフラグを立てる
	{
		if (const auto& owner = GetOwner())
		{
			for (const auto& chilled : owner->GetChildren())
			{
				if (auto chilled_transform = chilled->GetComponent<Transform2DComponent>())
				{
					chilled_transform->SetWorldDirtyFlag();
				}
			}
		}
	}

	// ローカル座標を返す
	return this->param.local_position;
}

void Transform2DComponent::SetLocalAngle(float angle) noexcept
{
	// ローカルアングルを更新
	this->param.local_angle = angle;

	// ワールドおよびローカルパラメータの更新が必要なフラグを立てる
	this->world_dirty_flag = true;
	this->local_dirty_flag = true;

	// 子オブジェクトのトランスフォームに値更新が必要なフラグを立てる
	{
		if (const auto& owner = GetOwner())
		{
			for (const auto& chilled : owner->GetChildren())
			{
				if (auto chilled_transform = chilled->GetComponent<Transform2DComponent>())
				{
					chilled_transform->SetWorldDirtyFlag();
				}
			}
		}
	}
}

void Transform2DComponent::SetLocalScale(DirectX::XMFLOAT2 scale) noexcept
{
	// ローカルアングルを更新
	this->param.local_scale = scale;

	// ワールドおよびローカルパラメータの更新が必要なフラグを立てる
	this->world_dirty_flag = true;
	this->local_dirty_flag = true;

	// 子オブジェクトのトランスフォームに値更新が必要なフラグを立てる
	{
		if (const auto& owner = GetOwner())
		{
			for (const auto& chilled : owner->GetChildren())
			{
				if (auto chilled_transform = chilled->GetComponent<Transform2DComponent>())
				{
					chilled_transform->SetWorldDirtyFlag();
				}
			}
		}
	}
}

DirectX::XMFLOAT2 Transform2DComponent::GetWorldPosition()
{
	if (this->world_dirty_flag) UpdateWorldParam();
	return this->world_position;
}

DirectX::XMFLOAT2 Transform2DComponent::GetWorldScale()
{
	if (this->world_dirty_flag) UpdateWorldParam();
	return this->world_scale;
}

float Transform2DComponent::GetWorldAngle()
{
	if (this->world_dirty_flag) UpdateWorldParam();
	return world_angle;
}

#ifdef _DEBUG

void Transform2DComponent::DrawDebugGUI()
{
	// local_position設定
	if (this->param.enable_ndc_debug)
	{
		DirectX::XMFLOAT2 local_position = this->param.local_position;
		if (ImGui::DragFloat2("Local Position", &local_position.x, 0.005f))
		{
			SetLocalPosition(local_position);
		}
		if (ImGui::DragFloat2("Local Scale", &this->param.local_scale.x, 0.1f, 0.0f, 1000.0f))
		{
			this->local_dirty_flag = true;
			this->world_dirty_flag = true;
		}
	}
	else
	{
		DirectX::XMFLOAT2 local_position = this->param.local_position;
		if (ImGui::DragFloat2("Local Position", &local_position.x, 0.1f))
		{
			SetLocalPosition(local_position);
		}
		if (ImGui::DragFloat2("Local Scale", &this->param.local_scale.x, 0.1f))
		{
			this->local_dirty_flag = true;
			this->world_dirty_flag = true;
		}
	}

	float angle_degrees
	{
		DirectX::XMConvertToDegrees(this->param.local_angle)
	};
	if (ImGui::SliderFloat("Local Angle", &angle_degrees, 0, 360.0f))
	{
		this->param.local_angle =
		{
			DirectX::XMConvertToRadians(angle_degrees)
		};
		this->world_dirty_flag = true;
		this->local_dirty_flag = true;
	}

	DirectX::XMFLOAT2 dummy_float2 = this->world_position;
	ImGui::InputFloat2("World Position", &dummy_float2.x);
	dummy_float2 = this->world_scale;
	ImGui::InputFloat2("World Scale", &dummy_float2.x);
	float dummy_float= this->world_angle;
	ImGui::InputFloat("World Angle", &dummy_float);
}

void Transform2DComponent::DrawDebugPrimitive()
{
	DebugManager::Instance debug_manager = DebugManager::GetInstance();
	if (!debug_manager.Get()) return;
	DebugPrimitiveRenderer* debug_render = debug_manager->GetDebugPrimitiveRenderer();
	this->sphere_world_position.SetCenter({ GetWorldPosition().x,GetWorldPosition().y,0.0f });
	debug_render->DrawSphere(this->sphere_world_position);
}

void Transform2DComponent::DrawDebugPrimitiveGUI()
{
	this->sphere_world_position.DrawDebugGUI("Sphere World Position");
}

#endif // _DEBUG