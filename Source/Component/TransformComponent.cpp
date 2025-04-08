#include "TransformComponent.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG
#include <cmath>
#include "Object/Object.h"

#include "Debug/DebugManager.h"

Transform3DComponent::Transform3DComponent(Transform3DParam param)
	:param(param),default_param(param)
{
#ifdef _DEBUG
	this->sphere_world_position = SphereParam(DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), 0.1f);
	SetIsDebugPrimitive(false);
#endif // _DEBUG
}

void Transform3DComponent::Update(float elapsed_time)
{
	// ローカルトランスフォームの更新
	if (this->local_dirty_flag) UpdateLocalParam();

	if (this->world_dirty_flag)
	{
		// ワールドトランスフォームの更新
		UpdateWorldParam();
	}
}

void Transform3DComponent::UpdateWorldParam()
{
	// 親の行列取得
	MYMATRIX Parent_transform = DirectX::XMMatrixIdentity();
	if (const auto& owner = GetOwner())
	{
		if (const auto& parent = owner->GetParent())
		{
			if (auto parent_transform = parent->GetComponent(this->parent_transform_Wptr))
			{
				Parent_transform = parent_transform->GetWorldTransform();
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
		// ワールドポジションの設定
		UpdateWorldPosition(Parent_transform);
	}

	this->world_dirty_flag = false;	// フラグを解除
}

void Transform3DComponent::UpdateLocalParam()
{
	// ローカル行列作成
	MYMATRIX Local_transform;
	Local_transform.SetLocalMatrix(this->param.local_scale, this->param.local_angle, this->param.local_position);
	Local_transform.GetFlaot4x4(this->local_transform);

	this->local_dirty_flag = false;	// フラグを解除
}

const DirectX::XMFLOAT4X4& Transform3DComponent::GetWorldTransform()
{
	if(this->world_dirty_flag) UpdateWorldParam();
	return this->world_transform;
}

const DirectX::XMFLOAT4X4& Transform3DComponent::GetLocalTransform()
{
	if (this->local_dirty_flag) UpdateLocalParam();
	return this->local_transform;
}

void Transform3DComponent::SetLocalPosition(DirectX::XMFLOAT3 pos) noexcept
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
				if (auto chilled_transform = chilled->GetComponent<Transform3DComponent>())
				{
					chilled_transform->SetWorldDirtyFlag();
				}
			}
		}
	}
}

DirectX::XMFLOAT3 Transform3DComponent::AddLocalPosition(DirectX::XMFLOAT3 vec)
{
	// ローカル座標の更新
	MYVECTOR3 Pos = this->param.local_position;
	Pos += vec;
	Pos.GetFlaot3(this->param.local_position);

	// ワールドおよびローカルパラメータの更新が必要なフラグを立てる
	this->world_dirty_flag = true;
	this->local_dirty_flag = true;

	// 子オブジェクトのトランスフォームに値更新が必要なフラグを立てる
	{
		if (const auto& owner = GetOwner())
		{
			for (const auto& chilled : owner->GetChildren())
			{
				if (auto chilled_transform = chilled->GetComponent<Transform3DComponent>())
				{
					chilled_transform->SetWorldDirtyFlag();
				}
			}
		}
	}

	// ローカル座標を返す
	return this->param.local_position;
}

void Transform3DComponent::SetLocalAngle(DirectX::XMFLOAT3 angle) noexcept
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
				if (auto chilled_transform = chilled->GetComponent<Transform3DComponent>())
				{
					chilled_transform->SetWorldDirtyFlag();
				}
			}
		}
	}
}

void Transform3DComponent::SetLocalScale(DirectX::XMFLOAT3 scale) noexcept
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
				if (auto chilled_transform = chilled->GetComponent<Transform3DComponent>())
				{
					chilled_transform->SetWorldDirtyFlag();
				}
			}
		}
	}
}

DirectX::XMFLOAT3 Transform3DComponent::GetWorldPosition()
{
	if (this->world_dirty_flag) UpdateWorldParam();
	return this->world_position;
}

void Transform3DComponent::UpdateWorldPosition()
{
	MYMATRIX Parent_transform = DirectX::XMMatrixIdentity();
	const auto& owner = GetOwner();
	if (!owner)
	{
		this->world_position = this->param.local_position;;
		return;
	};
	const auto& parent = owner->GetParent();
	if (!parent)
	{
		this->world_position = this->param.local_position;;
		return;
	};
	auto parent_ransform = parent->GetComponent(this->parent_transform_Wptr);
	if (!parent_ransform)
	{
		this->world_position = this->param.local_position;;
		return;
	}
	Parent_transform = parent_ransform->GetWorldTransform();
	UpdateWorldPosition(Parent_transform);
}

void Transform3DComponent::UpdateWorldPosition(MYMATRIX Parent_transform)
{
	MYVECTOR3 Local_position = this->param.local_position;

	MYVECTOR3 Worldl_position = Parent_transform.Vector3TransformCoord(Local_position);
	Worldl_position.GetFlaot3(this->world_position);
}

#ifdef _DEBUG

void Transform3DComponent::DrawDebugGUI()
{
	// local_position設定
	{
		DirectX::XMFLOAT3 local_position = this->param.local_position;
		if (ImGui::DragFloat3("Local Position", &local_position.x, 0.1f))
		{
			SetLocalPosition(local_position);
		}
	}
	if (ImGui::DragFloat3("Local Scale", &this->param.local_scale.x, 0.1f))
	{
		this->local_dirty_flag = true;
		this->world_dirty_flag = true;
	}

	DirectX::XMFLOAT3 angle_degrees
	{
		DirectX::XMConvertToDegrees(this->param.local_angle.x),
		DirectX::XMConvertToDegrees(this->param.local_angle.y),
		DirectX::XMConvertToDegrees(this->param.local_angle.z),
	};
	if (ImGui::SliderFloat3("Local Angle", &angle_degrees.x, 0, 360.0f))
	{
		this->param.local_angle =
		{
			DirectX::XMConvertToRadians(angle_degrees.x),
			DirectX::XMConvertToRadians(angle_degrees.y),
			DirectX::XMConvertToRadians(angle_degrees.z),
		};
		this->world_dirty_flag = true;
		this->local_dirty_flag = true;
	}

	DirectX::XMFLOAT3 dummy_float3 = this->world_position;
	ImGui::InputFloat3("World Position", &dummy_float3.x);
}

void Transform3DComponent::DrawDebugPrimitive()
{
	DebugManager::Instance debug_manager = DebugManager::GetInstance();
	if (!debug_manager.Get()) return;
	DebugPrimitiveRenderer* debug_render = debug_manager->GetDebugPrimitiveRenderer();
	this->sphere_world_position.SetCenter(GetWorldPosition());
	debug_render->DrawSphere(this->sphere_world_position);
}

void Transform3DComponent::DrawDebugPrimitiveGUI()
{
	this->sphere_world_position.DrawDebugGUI("Sphere World Position");
}

#endif // _DEBUG