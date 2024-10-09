#include "System/MyMath/MYMATRIX.h"
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
	MYVECTOR3 focus = transform->GetPosition();
	MYVECTOR3 eye = camera->GetEye();

	// �����s��𐶐�
	MYMATRIX V;
	{
		MYVECTOR3 up(0.0f, 1.0f, 0.0f);
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
				MYVECTOR3 forward = camera->GetViewTransform().GetForward();
				MYVECTOR3 right = camera->GetViewTransform().GetRight();
				if (GetKeyState('W') < 0) 
				{
					focus += forward * 10.f * elapsed_time;
				}
				if (GetKeyState('S') < 0) 
				{
					focus -= forward * 10.f * elapsed_time;
				}
				if (GetKeyState('A') < 0) 
				{
					focus -= right * 10.f * elapsed_time;
				}
				if (GetKeyState('D') < 0) 
				{
					focus += right * 10.f * elapsed_time;
				}
				if (GetKeyState('E') < 0) 
				{
					focus += MYVECTOR3(0.0f, 10.0f * elapsed_time, 0.0f);
				}
				if (GetKeyState('Q') < 0) 
				{
					focus += MYVECTOR3(0.0f, 10.0f * elapsed_time, 0.0f);
				}
			}
			else if (::GetAsyncKeyState(VK_MBUTTON) & 0x8000)
			{
				V.SetLookAtLH(eye, focus, up);

				MYMATRIX W = V.GetInverse(nullptr);
				// ���s�ړ�
				float s = range * 0.035f;
				float x = moveX * s;
				float y = moveY * s;
				focus -= W.GetRight() * x;
				focus += W.GetUp() * y;
			}
			if (mouse.GetWheel() != 0)	// �Y�[��
			{
				range -= static_cast<float>(mouse.GetWheel()) * range * 0.00000001f;
			}
		}
		float sx = ::sinf(rotateX), cx = ::cosf(rotateX);
		float sy = ::sinf(rotateY), cy = ::cosf(rotateY);
		MYVECTOR3 front(-cx * sy, -sx, -cx * cy);
		front *= range;
		eye = focus - front;

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
	MYVECTOR3 target = camera->GetTarget();

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
	MYMATRIX transform;
	transform.SetRotationRollPitchYaw(rotateX, rotateY, 0.0f);

	// ��]�s�񂩂�O���x�N�g�������o��
	MYVECTOR3 front(transform.GetForward());

	// �����_������̃x�N�g�������Ɉ�藣�ꂽ�J�������_�����߂�
	MYVECTOR3 eye = target - front * range;

	// �J�����̎��_�ƒ����_��ݒ�
	camera->SetLookAt(eye, target, MYVECTOR3(0.0f, 1.0f, 0.0f));
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