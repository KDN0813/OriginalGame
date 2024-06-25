#include "Camera.h"

Camera::Camera()
    : view()
    , projection()
    , eye()
    , focus()
    , up()
    , front()
    , right()
{
}

void Camera::SetLookAt(
    const DirectX::XMFLOAT3& eye
    , const DirectX::XMFLOAT3& focus
    , const DirectX::XMFLOAT3& up
)
{
    // 視点、中視点、上方向からビューを行列を作成
    DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
    DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
    DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
    DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye,Focus,Up);
    DirectX::XMStoreFloat4x4(&view, View);

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

void Camera::SetPerspectiveFov(float fovY, float aspect, float nearX, float farZ)
{
    DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearX, farZ);
    DirectX::XMStoreFloat4x4(&projection, Projection);
}
