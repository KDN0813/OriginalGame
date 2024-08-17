#include <imgui.h>
#include "CameraComponent.h"
#include "Object/Object.h"
#include "Camera/CameraManager.h"

#include "Component/TransformComponent.h"

CameraComponent::CameraComponent(CameraManager* camera_manager)
    :camera_manager(camera_manager)
{
    SetLookAt(
        DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
        DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
        DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)
    );

    this->camera_manager->AddCamera(this);
}

CameraComponent::~CameraComponent()
{
    this->camera_manager->RemoveCamera(this);
}

void CameraComponent::Update(float elapsed_time)
{
    auto owner = GetOwner();
    DirectX::XMFLOAT3 target;
    auto transform = owner->GetComponent<Transform3DComponent>(transform_Wptr);

    target = transform ? transform->GetPosition() : DirectX::XMFLOAT3();

    DirectX::XMFLOAT3 set_eye =
    {
        set_eye.x = target.x - up.x * range.x - front.x * range.x - right.x * range.x,
        set_eye.y = target.y - up.y * range.y - front.y * range.y - right.y * range.y,
        set_eye.z = target.z - up.z * range.z - front.z * range.z - right.z * range.z,
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
    float fovY_degrees = DirectX::XMConvertToDegrees(fovY);
    if (ImGui::SliderFloat("fovY", &fovY_degrees, 0, 260.0f))
    {
        fovY = DirectX::XMConvertToRadians(fovY_degrees);
    }

    ImGui::InputFloat("nearZ", &this->nearZ);
    ImGui::InputFloat("farZ", &this->farZ);
    ImGui::InputFloat3("focus", &this->farZ);
    ImGui::InputFloat3("range", &this->farZ);

    if (ImGui::Button("SetMainCamera"))
    {
        SetMainCamera();
    }
}

#endif // _DEBUG