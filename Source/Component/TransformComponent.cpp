#include "TransformComponent.h"
#include <imgui.h>

#include "Debug/DebugManager.h"

void Transform3DComponent::Update(float elapsed_time)
{
	// 値が変更されていなければ処理しない
	if (!this->change_value) return;

	// ワールド行列の更新
	MYMATRIX S, R, T;
	S.SetScalingMatrix(this->scale);
	R.SetRotationRollPitchYaw(this->angle);
	T.SetTranslationMatrix(this->position);

	this->transform = S * R * T;
	this->change_value = false;

	{
		DebugRenderer* debug_render = DebugManager::Instance()->GetDebugRenderer();
		debug_render->DrawSphere(position.GetFlaot3(), 0.06f, DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f));
	}
}

MYVECTOR3 Transform3DComponent::AddPosition(MYVECTOR3 vec)
{
	this->change_value = true;
	this->position += vec;
	return this->position;
}

#ifdef _DEBUG

void Transform3DComponent::DrawDebugGUI()
{
	if (this->position.InputFloat("position"))
	{
		this->change_value = true;
	}
	if (this->scale.InputFloat("scale"))
	{
		this->change_value = true;
	}

	MYVECTOR3 angle_degrees = 
	{
		DirectX::XMConvertToDegrees(this->angle.GetX()),
		DirectX::XMConvertToDegrees(this->angle.GetY()),
		DirectX::XMConvertToDegrees(this->angle.GetZ()),
	};
	if (angle_degrees.SliderFloat("angle", 0, 360.0f))
	{
		this->angle =
		{
			DirectX::XMConvertToRadians(angle_degrees.GetX()),
			DirectX::XMConvertToRadians(angle_degrees.GetY()),
			DirectX::XMConvertToRadians(angle_degrees.GetZ()),
		};
		this->change_value = true;
	}
}

#endif // _DEBUG