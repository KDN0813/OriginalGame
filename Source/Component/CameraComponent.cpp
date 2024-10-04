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
    auto transform = owner->EnsureComponentValid<Transform3DComponent>(transform_Wptr);

    target = transform ? transform->GetPosition() : DirectX::XMFLOAT3();

    float sx = ::sinf(rotateX), cx = ::cosf(rotateX);
    float sy = ::sinf(rotateY), cy = ::cosf(rotateY);
    DirectX::XMFLOAT3 front =
    {
        -cx * sy,
        -sx,
        -cx * cy
    };

    DirectX::XMFLOAT3 set_eye =
    {
        target.x - front.x * range,
        target.y - front.y * range,
        target.z - front.z * range
    };

    SetLookAt(set_eye, target, DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));
}

void CameraComponent::SetMainCamera()
{
    this->camera_manager->SetMainCamera(this);
}

void CameraComponent::SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up)
{
    // 視点、中視点、上方向からビューを行列を作成
    DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
    DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
    DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
    DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
    DirectX::XMStoreFloat4x4(&view_transform, View);

    // ビューを逆行列化し、ワールド行列に戻す
    DirectX::XMMATRIX World = DirectX::XMMatrixInverse(nullptr, View);
    DirectX::XMFLOAT4X4 world;
    DirectX::XMStoreFloat4x4(&world, World);

    // カメラ方向を取り出す
    this->right.x = world._11;
    this->right.y = world._12;
    this->right.z = world._13;

    this->up.x = world._21;
    this->up.y = world._22;
    this->up.z = world._23;

    this->front.x = world._31;
    this->front.y = world._32;
    this->front.z = world._33;

    // 視点、注視点を保存
    this->eye = eye;
    this->focus = focus;
}

void CameraComponent::SetPerspectiveFov(float fovY, float aspect, float nearX, float farZ)
{
    // が書く、画面比率、クリップ距離からプロジェクション行列を作成
    DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearX, farZ);
    DirectX::XMStoreFloat4x4(&projection_transform, Projection);
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
}

#endif // _DEBUG