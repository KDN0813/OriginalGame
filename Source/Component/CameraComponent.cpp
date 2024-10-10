#include <imgui.h>
#include "System/MyMath/MYMATRIX.h"
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
        DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),
        DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
        DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)
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
    target = transform ? transform->GetPosition() : DirectX::XMFLOAT3();

    float sx = ::sinf(rotateX), cx = ::cosf(rotateX);
    float sy = ::sinf(rotateY), cy = ::cosf(rotateY);
    MYVECTOR3 Front = MYVECTOR3(-cx * sy, -sx, -cx * cy);
    MYVECTOR3 Target = target;
    MYVECTOR3 Eye = Target - Front * range;
    MYVECTOR3 Up = MYVECTOR3(0.0f, 1.0f, 0.0f);

    SetLookAt(this->forward, target, this->eye);
}

void CameraComponent::SetMainCamera()
{
    this->camera_manager->SetMainCamera(this);
}

void CameraComponent::SetLookAt(DirectX::XMFLOAT3 eye, DirectX::XMFLOAT3 focus, DirectX::XMFLOAT3 up)
{
    // 視点、中視点、上方向からビューを行列を作成
    MYMATRIX View_transform = this->view_transform;
    View_transform.SetLookAtLH(eye, focus, up);
    this->view_transform = View_transform.GetFlaot4x4();

    // ビューを逆行列化し、ワールド行列に戻す
    MYMATRIX World_transform = View_transform.GetInverse(nullptr);
    this->world_transform = World_transform.GetFlaot4x4();

    // カメラ方向を取り出す
    this->right.x = this->world_transform._11;
    this->right.y = this->world_transform._12;
    this->right.z = this->world_transform._13;

    this->up.x = this->world_transform._21;
    this->up.y = this->world_transform._22;
    this->up.z = this->world_transform._23;

    this->forward.x = this->world_transform._31;
    this->forward.y = this->world_transform._32;
    this->forward.z = this->world_transform._33;

    // 視点、注視点を保存
    this->eye = eye;
    this->focus = focus;
}

void CameraComponent::SetPerspectiveFov(float fovY, float aspect, float nearX, float farZ)
{
    // 画面比率、クリップ距離からプロジェクション行列を作成
    MYMATRIX Projection_transform = this->projection_transform;
    Projection_transform.SetPerspectiveFovLH(fovY, aspect, nearX, farZ);
    this->projection_transform = Projection_transform.GetFlaot4x4();
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