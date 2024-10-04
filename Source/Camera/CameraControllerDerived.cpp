#include <DirectXMath.h>
#include <imgui.h>
#include "Input/Input.h"
#include "CameraControllerDerived.h"
#include "Object/Object.h"
#include "Component/CameraComponent.h"

#ifdef _DEBUG
#include "Component/TransformComponent.h"

void DebugCameraController::Update(float elapsed_time)
{
	auto owner = this->owner_Wptr.lock();
	if (!owner) return ;
	auto camera = owner->EnsureComponentValid<CameraComponent>(this->camera_Wptr);
	if (!camera) return;
	if (!camera->GetIsMainCamera()) return;
	auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr);

	Mouse& mouse = Input::Instance()->GetMouse();

	float moveX = (mouse.GetPositionX() - mouse.GetOldPositionX()) * 0.02f;
	float moveY = (mouse.GetPositionY() - mouse.GetOldPositionY()) * 0.02f;

	float rotateX = camera->GetRotateX();
	float rotateY = camera->GetRotateY();
	float range = camera->GetRange();
	DirectX::XMFLOAT3 focus = transform->GetPosition();
	DirectX::XMFLOAT3 eye = camera->GetEye();

	// �����s��𐶐�
	DirectX::XMMATRIX V;
	{
		DirectX::XMVECTOR up{ DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) };
		// �}�E�X����
		{
			if (::GetAsyncKeyState(VK_RBUTTON) & 0x8000)
			{
				// Y����]
				rotateY += moveX * 0.5f;
				if (rotateY > DirectX::XM_PI)
					rotateY -= DirectX::XM_2PI;
				else if (rotateY < -DirectX::XM_PI)
					rotateY += DirectX::XM_2PI;
				// X����]
				rotateX += moveY * 0.5f;
				if (rotateX > DirectX::XMConvertToRadians(89.9f))
					rotateX = DirectX::XMConvertToRadians(89.9f);
				else if (rotateX < -DirectX::XMConvertToRadians(89.9f))
					rotateX = -DirectX::XMConvertToRadians(89.9f);

				// �L�[�ړ�
				DirectX::XMFLOAT4X4 vm = camera->GetViewTransform();
				DirectX::XMFLOAT3 forward(vm._13, vm._23, vm._33);
				DirectX::XMFLOAT3 right(vm._11, vm._21, vm._31);
				if (GetKeyState('W') < 0) 
				{
					focus.x += forward.x * 10 * elapsed_time;
					focus.y += forward.y * 10 * elapsed_time;
					focus.z += forward.z * 10 * elapsed_time;
				}
				if (GetKeyState('S') < 0) 
				{
					focus.x -= forward.x * 10 * elapsed_time;
					focus.y -= forward.y * 10 * elapsed_time;
					focus.z -= forward.z * 10 * elapsed_time;
				}
				if (GetKeyState('A') < 0) 
				{
					focus.x -= right.x * 10 * elapsed_time;
					focus.y -= right.y * 10 * elapsed_time;
					focus.z -= right.z * 10 * elapsed_time;
				}
				if (GetKeyState('D') < 0) 
				{
					focus.x += right.x * 10 * elapsed_time;
					focus.y += right.y * 10 * elapsed_time;
					focus.z += right.z * 10 * elapsed_time;
				}
				if (GetKeyState('E') < 0) 
				{
					focus.y += 10 * elapsed_time;
				}
				if (GetKeyState('Q') < 0) 
				{
					focus.y -= 10 * elapsed_time;
				}
			}
			else if (::GetAsyncKeyState(VK_MBUTTON) & 0x8000)
			{
				V = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eye),
					DirectX::XMLoadFloat3(&focus),
					up);
				DirectX::XMFLOAT4X4 W;
				DirectX::XMStoreFloat4x4(&W, DirectX::XMMatrixInverse(nullptr, V));
				// ���s�ړ�
				float s = range * 0.035f;
				float x = moveX * s;
				float y = moveY * s;
				focus.x -= W._11 * x;
				focus.y -= W._12 * x;
				focus.z -= W._13 * x;

				focus.x += W._21 * y;
				focus.y += W._22 * y;
				focus.z += W._23 * y;
			}
			if (mouse.GetWheel() != 0)	// �Y�[��
			{
				range -= static_cast<float>(mouse.GetWheel()) * range * 0.00000001f;
			}
		}
		float sx = ::sinf(rotateX), cx = ::cosf(rotateX);
		float sy = ::sinf(rotateY), cy = ::cosf(rotateY);
		DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
		DirectX::XMVECTOR Front = DirectX::XMVectorSet(-cx * sy, -sx, -cx * cy, 0.0f);
		DirectX::XMVECTOR Range = DirectX::XMVectorSet(range, range, range, 0.0f);
		Front = DirectX::XMVectorMultiply(Front, Range);
		DirectX::XMVECTOR Eye = DirectX::XMVectorSubtract(Focus, Front);
		DirectX::XMStoreFloat3(&eye, Eye);


		// �J�����Ɏ��_�𒍎��_��ݒ�
		camera->SetLookAt(eye, focus, { 0, 1, 0 });
	}

	if (mouse.GetWheel() != 0)
	{
		// �Y�[��
		range -= static_cast<float>(mouse.GetWheel()) * range * 0.001f;
	}

	camera->SetRotateX(rotateX);
	camera->SetRotateY(rotateY);
	camera->SetRange(range);

	transform->SetPosition(focus);	// Position�̍Đݒ�
}

#endif // _DEBUG

GamepadCameraController::GamepadCameraController(OwnerPtr owner, float rollSpeed)
	:CameraControllerBase(owner),
	roll_speed(DirectX::XMConvertToRadians(rollSpeed))
{
}

void GamepadCameraController::Update(float elapsed_time)
{
	auto owner = this->owner_Wptr.lock();
	if (!owner) return;
	auto camera = owner->EnsureComponentValid<CameraComponent>(this->camera_Wptr);
	if (!camera) return;
	if (!camera->GetIsMainCamera()) return;

	float rotateX = camera->GetRotateX();
	float rotateY = camera->GetRotateY();
	float range = camera->GetRange();
	DirectX::XMFLOAT3 target = camera->GetTarget();

	GamePad& gamePad = Input::Instance()->GetGamePad();
	float ax = gamePad.GetAxisRX();
	float ay = gamePad.GetAxisRY();
	// �J�����̉�]���x
	float speed = this->roll_speed * elapsed_time;

	// �X�e�B�b�N���͒l�ɍ��킹��X����Y������]
	rotateY += ax * speed;

	if (rotateY < -DirectX::XM_PI) rotateY += DirectX::XM_2PI;
	if (rotateY > DirectX::XM_PI) rotateY -= DirectX::XM_2PI;

	// �J������]�l����]�s��ɕϊ�
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(rotateX, rotateY, 0.0f);

	// ��]�s�񂩂�O���x�N�g�������o��
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	// �����_������̃x�N�g�������Ɉ�藣�ꂽ�J�������_�����߂�
	DirectX::XMFLOAT3 eye;

	eye.x = target.x - front.x * range;
	eye.y = target.y - front.y * range;
	eye.z = target.z - front.z * range;

	// �J�����̎��_�ƒ����_��ݒ�
	camera->SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
	camera->SetRotateY(rotateY);
}

#ifdef _DEBUG
void GamepadCameraController::DrawDebugGUI()
{
	float roll_speed_deg = DirectX::XMConvertToDegrees(this->roll_speed);
	if (ImGui::SliderFloat("RollSpeed", &roll_speed_deg, 0.0f, 180.0f))
	{
		this->roll_speed = DirectX::XMConvertToRadians(roll_speed_deg);
	}
}

#endif // _DEBUG