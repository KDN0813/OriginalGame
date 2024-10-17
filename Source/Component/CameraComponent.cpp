#include <imgui.h>
#include "System/MyMath/MYMATRIX.h"
#include "CameraComponent.h"
#include "Object/Object.h"
#include "Camera/CameraManager.h"

#include "Component/TransformComponent.h"

CameraComponent::CameraComponent(CameraParam camera_param, CameraManager* camera_manager)
    :camera_manager(camera_manager),camera_param(camera_param),default_param(camera_param)
{
    this->camera_manager->AddCamera(this);
}

void CameraComponent::Start()
{
}

void CameraComponent::End()
{
    this->camera_manager->RemoveCamera(this);
}

void CameraComponent::ReStart()
{
    this->camera_param = this->default_param;
    change_value = true;
}

void CameraComponent::Update(float elapsed_time)
{
    if (!this->change_value) return;

    float sx = ::sinf(this->camera_param.rotateX), cx = ::cosf(this->camera_param.rotateX);
    float sy = ::sinf(this->camera_param.rotateY), cy = ::cosf(this->camera_param.rotateY);
    MYVECTOR3 Front = MYVECTOR3(-cx * sy, -sx, -cx * cy);
    MYVECTOR3 Focus = this->camera_param.focus;
    MYVECTOR3 Eye = Focus - Front * this->camera_param.range;
    MYVECTOR3 Up = MYVECTOR3(0.0f, 1.0f, 0.0f);

    SetLookAt(Eye, Focus, Up);
    SetPerspectiveFov(this->camera_param.fovY, this->camera_param.aspect, this->camera_param.nearZ, this->camera_param.farZ);
}

void CameraComponent::SetMainCamera()
{
    this->camera_manager->SetMainCamera(this);
}

void CameraComponent::SetLookAt(MYVECTOR3 Eye, MYVECTOR3 Focus, MYVECTOR3 Up)
{
    // 視点、中視点、上方向からビューを行列を作成
    MYMATRIX View_transform;
    View_transform.SetLookAtLH(Eye, Focus, Up);
    View_transform.GetFlaot4x4(this->view_transform);

    // ビューを逆行列化し、ワールド行列に戻す
    MYMATRIX World_transform = View_transform.GetInverse(nullptr);

    // カメラ方向を取り出す
    World_transform.GetRight().GetFlaot3(this->camera_param.right);
    World_transform.GetUp().GetFlaot3(this->camera_param.up);
    World_transform.GetForward().GetFlaot3(this->camera_param.forward);

    // 視点、注視点を保存
    Eye.GetFlaot3(this->camera_param.eye);
    Focus.GetFlaot3(this->camera_param.focus);
}

void CameraComponent::SetPerspectiveFov(float fovY, float aspect, float nearX, float farZ)
{
    // 画面比率、クリップ距離からプロジェクション行列を作成
    MYMATRIX Projection_transform;
    Projection_transform.SetPerspectiveFovLH(fovY, aspect, nearX, farZ);
    Projection_transform.GetFlaot4x4(this->projection_transform);
}

#ifdef _DEBUG

void CameraComponent::DrawDebugGUI()
{
    if (ImGui::SliderFloat("front_range", &this->camera_param.range, 1.0f, 50.0f))
    {
        this->change_value = true;
    }
    if (ImGui::SliderFloat("rotateY", &this->camera_param.rotateY, -DirectX::XM_PI, DirectX::XM_PI))
    {
        this->change_value = true;
    }
    constexpr float RotateX_Max = DirectX::XMConvertToRadians(89.0f);
    if (ImGui::SliderFloat("rotateX", &this->camera_param.rotateX, -RotateX_Max, RotateX_Max))
    {
        this->change_value = true;
    }

    if (ImGui::InputFloat3("focus", &this->camera_param.focus.x))
    {
        this->change_value = true;
    }

    ImGui::Checkbox("Is Main Camera", &this->is_main_camera);

    if (ImGui::Button("ReStart"))
    {
        ReStart();
    }

    if (ImGui::Button("SetMainCamera"))
    {
        SetMainCamera();
    }
}

#endif // _DEBUG