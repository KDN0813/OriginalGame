#include "System/MyMath/MYMATRIX.h"
#ifdef _DEBUG
#include "Debug\ImGuiHelper.h"
#endif // _DEBUG
#include "Input/Input.h"
#include "CameraControllerComponent.h"
#include "Object/Object.h"
#include "Component/CameraComponent.h"
#include "Component/TransformComponent.h"

CameraControllerGamepad::CameraControllerGamepad(CameraControllerParam param)
	: param(param),default_param(param)
{
}

void CameraControllerGamepad::ReStart()
{
	this->param = default_param;
}

void CameraControllerGamepad::Update(float elapsed_time)
{
	auto owner = GetOwner();
	if (!owner) return;
	auto camera = owner->EnsureComponentValid<CameraComponent>(this->camera_Wptr);
	if (!camera) return;
	if (!camera->GetIsActive()) return;
	DirectX::XMFLOAT3 focus{};
	{
		auto transform = owner->EnsureComponentValid<Transform3DComponent>(this->transform_Wptr);
		if (transform)
		{
			focus = transform->GetWorldPosition();
		}
	}

	float rotateX = camera->GetRotateX();
	float rotateY = camera->GetRotateY();
	float range = camera->GetRange();
	MYVECTOR3 Focus = focus;

	// ���͏������o��
	float ax{};
	float ay{};
	if (Input::Instance input = Input::GetInstance(); input.Get())
	{
		GamePad& gamePad = input->GetGamePad();
		ax = gamePad.GetAxisRX();
		ay = gamePad.GetAxisRY();
	}
	// �J�����̉�]���x
	float speed = this->param.roll_speed * elapsed_time;

	// �X�e�B�b�N���͒l�ɍ��킹��X����Y������]
	rotateY += ax * speed;
	rotateX += ay * speed;

	if (rotateX < this->param.min_angleX) rotateX = this->param.min_angleX;
	if (rotateX > this->param.max_angleX) rotateX = this->param.max_angleX;
	if (rotateY < -DirectX::XM_PI) rotateY += DirectX::XM_2PI;
	if (rotateY > DirectX::XM_PI) rotateY -= DirectX::XM_2PI;

	// �J������]�l����]�s��ɕϊ�
	MYMATRIX Transform;
	Transform.SetRotationRollPitchYaw(rotateX, rotateY, 0.0f);
	DirectX::XMMATRIX transform = Transform.GetMatrix();

	// ��]�s�񂩂�O���x�N�g�������o��
	MYVECTOR3 Front = transform.r[2];

	// �����_������̃x�N�g�������Ɉ�藣�ꂽ�J�������_�����߂�
	MYVECTOR3 Eye = Focus - Front * range;

	DirectX::XMFLOAT3 eye;
	Eye.GetFlaot3(eye);
	Focus.GetFlaot3(focus);

	// �J�����̎��_�ƒ����_��ݒ�
	camera->SetFocus(focus);
	camera->SetEye(eye);
	camera->SetRotateX(rotateX);
	camera->SetRotateY(rotateY);
}

#if defined(_DEBUG) || defined(RELEASE_DEBUG)
#include "Component/TransformComponent.h"

void CameraControllerDebug::Update(float elapsed_time)
{
	if (!(GetKeyState(VK_CONTROL) & 0x8000)) return;

	auto owner = GetOwner();
	if (!owner) return;
	auto camera = owner->EnsureComponentValid<CameraComponent>(this->camera_Wptr);
	if (!camera) return;
	if (!camera->GetIsActive()) return;

	// �}�E�X���擾
	Input::Instance input = Input::GetInstance();
	if (!input.Get()) return;
	Mouse& mouse = input->GetMouse();
	float moveX = (mouse.GetPositionX() - mouse.GetOldPositionX()) * 0.02f;
	float moveY = (mouse.GetPositionY() - mouse.GetOldPositionY()) * 0.02f;

	float rotateX = camera->GetRotateX();
	float rotateY = camera->GetRotateY();
	float range = camera->GetRange();
	MYVECTOR3 Focus = camera->GetFocus();
	MYVECTOR3 Eye = camera->GetEye();

	// �����s��𐶐�
	MYMATRIX V;
	{
		const MYVECTOR3 Up(0.0f, 1.0f, 0.0f);
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
				MYVECTOR3 Forward = camera->GetForward();
				MYVECTOR3 Right = camera->GetRight();
				if (GetKeyState('W') < 0)
				{
					Focus += Forward * 10.f * elapsed_time;
				}
				if (GetKeyState('S') < 0)
				{
					Focus -= Forward * 10.f * elapsed_time;
				}
				if (GetKeyState('A') < 0)
				{
					Focus -= Right * 10.f * elapsed_time;
				}
				if (GetKeyState('D') < 0)
				{
					Focus += Right * 10.f * elapsed_time;
				}
				if (GetKeyState('E') < 0)
				{
					Focus.AddY(10.0f * elapsed_time);
				}
				if (GetKeyState('Q') < 0)
				{
					Focus.SubtractY(10.0f * elapsed_time);
				}
			}
			else if (::GetAsyncKeyState(VK_MBUTTON) & 0x8000)
			{
				V.SetLookAtLH(Eye, Focus, Up);

				MYMATRIX W = V.GetInverse(nullptr);
				// ���s�ړ�
				float s = range * 0.035f;
				float x = moveX * s;
				float y = moveY * s;

				MYVECTOR3 W_right = W.GetRight();
				MYVECTOR3 W_up = W.GetUp();

				Focus -= W_right * x;
				Focus += W_up * y;
			}
			if (mouse.GetWheel() != 0)	// �Y�[��
			{
				range -= static_cast<float>(mouse.GetWheel()) * range * 0.00000001f;
			}
		}
		float sx = ::sinf(rotateX), cx = ::cosf(rotateX);
		float sy = ::sinf(rotateY), cy = ::cosf(rotateY);
		MYVECTOR3 Front(-cx * sy, -sx, -cx * cy);
		Front *= range;
		Eye = Focus - Front;
	}

	if (mouse.GetWheel() != 0)
	{
		// �Y�[��
		range -= static_cast<float>(mouse.GetWheel()) * range * 0.001f;
	}

	camera->SetRotateX(rotateX);
	camera->SetRotateY(rotateY);
	camera->SetRange(range);

	DirectX::XMFLOAT3 focus{};
	Focus.GetFlaot3(focus);
	camera->SetFocus(focus);
	DirectX::XMFLOAT3 eye;
	Eye.GetFlaot3(eye);
	camera->SetEye(eye);
}

#ifdef _DEBUG

void CameraControllerGamepad::DrawDebugGUI()
{
	float roll_speed_deg = DirectX::XMConvertToDegrees(this->param.roll_speed);
	if (ImGui::SliderFloat("RollSpeed", &roll_speed_deg, 0.0f, 180.0f))
	{
		this->param.roll_speed = DirectX::XMConvertToRadians(roll_speed_deg);
	}
	ImGui::DragAngleSlider("Max AngleX", this->param.max_angleX);
	ImGui::DragAngleSlider("Min AngleX", this->param.min_angleX);
	
}
#endif // DEBUG

#endif // _DEBUG
