#include <imgui.h>
#include "CameraComponent.h"
#include "Object/Object.h"
#include "Camera/CameraManager.h"
#include "Camera/CameraController.h"

#include "Component/TransformComponent.h"

CameraComponent::CameraComponent(CameraManager* camera_manager)
    :camera_manager(camera_manager)
{
    this->camera_manager->AddCamera(this);
}

void CameraComponent::SetCameraController(std::unique_ptr<CameraControllerBase> camera_controller)
{
    this->camera_controller = std::move(camera_controller);
}

void CameraComponent::Start()
{
    SetLookAt(
        MYVECTOR3(0.0f, 0.0f, -1.0f),
        MYVECTOR3(0.0f, 0.0f, 0.0f),
        MYVECTOR3(0.0f, 1.0f, 0.0f)
    );
}

void CameraComponent::End()
{
    this->camera_manager->RemoveCamera(this);
}

void CameraComponent::Update(float elapsed_time)
{
    if (this->camera_controller)
    {
        this->camera_controller->Update(elapsed_time);
    }
    auto owner = GetOwner();
    if (!owner) return;
    auto transform = owner->EnsureComponentValid<Transform3DComponent>(transform_Wptr);
    target = transform ? transform->GetPosition() : MYVECTOR3();

    float sx = ::sinf(rotateX), cx = ::cosf(rotateX);
    float sy = ::sinf(rotateY), cy = ::cosf(rotateY);
    MYVECTOR3 front = MYVECTOR3(-cx * sy, -sx, -cx * cy);
    MYVECTOR3 set_eye = target - front * range;
    MYVECTOR3 up = MYVECTOR3(0.0f, 1.0f, 0.0f);
    SetLookAt(set_eye, target, up);
}

void CameraComponent::SetMainCamera()
{
    this->camera_manager->SetMainCamera(this);
}

void CameraComponent::SetLookAt(MYVECTOR3 eye, MYVECTOR3 focus, MYVECTOR3 up)
{
    // ���_�A�����_�A���������r���[���s����쐬
    this->view_transform.SetLookAtLH(eye, focus, up);

    // �r���[���t�s�񉻂��A���[���h�s��ɖ߂�
    this->world_transform = this->view_transform.GetInverse(nullptr);

    // ���_�A�����_��ۑ�
    this->eye = eye;
    this->focus = focus;
}

void CameraComponent::SetPerspectiveFov(float fovY, float aspect, float nearX, float farZ)
{
    // ��ʔ䗦�A�N���b�v��������v���W�F�N�V�����s����쐬
    this->projection_transform.SetPerspectiveFovLH(fovY, aspect, nearX, farZ);
}

#ifdef _DEBUG

void CameraComponent::DrawDebugGUI()
{
    ImGui::SliderFloat("front_range", &this->range, 1.0f, 50.0f);
    ImGui::SliderFloat("rotateY", &rotateY, -DirectX::XM_PI, DirectX::XM_PI);
    constexpr float RotateX_Max = DirectX::XMConvertToRadians(89.0f);
    ImGui::SliderFloat("rotateX", &rotateX, -RotateX_Max, RotateX_Max);
    ImGui::Checkbox("Is Main Camera", &this->is_main_camera);

    if (ImGui::Button("SetMainCamera"))
    {
        SetMainCamera();
    }

    if (this->camera_controller)
    {
        if (ImGui::CollapsingHeader("Camera Controller", ImGuiTreeNodeFlags_DefaultOpen))
        {
            char buffer[1024];
            ::strncpy_s(buffer, sizeof(buffer), this->camera_controller->GetName(), sizeof(buffer));
            ImGui::InputText("name", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue);

            this->camera_controller->DrawDebugGUI();
        }
    }
}

#endif // _DEBUG