#include "DebugObject.h"
#include <imgui.h>

#include "Graphics/Graphics.h"
#include "Camera/Camera.h"

DebugObject::DebugObject(const char* filename)
    :model(std::make_unique<Model>(filename))
    , position()
    , angle()
    , scale({ 0.01f,0.01f ,0.01f })
    , transform()
{
}

void DebugObject::Update(float elapsedTime)
{
    this->UpdateTransform();

    this->model->UpdateTransform(this->transform);
}

void DebugObject::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, this->model.get());
}

void DebugObject::DrawImGUi()
{
    if (ImGui::Begin("DebugObject"))
    {
        ImGui::InputFloat3("position", &this->position.x);
        ImGui::InputFloat3("angle", &this->angle.x);
        ImGui::InputFloat3("scale", &this->scale.x);
    }
    ImGui::End();
}

void DebugObject::UpdateTransform()
{
    // スケール行列を作成
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    //回転行列を作成
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    // 位置行列を作成
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    // 3つの行列を組み合わせ、ワールド行列を作成
    DirectX::XMMATRIX W = S * R * T;
    // 計算したワールド行列を取り出す
    DirectX::XMStoreFloat4x4(&transform, W);
}