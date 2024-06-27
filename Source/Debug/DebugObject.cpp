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

void DebugObject::Render(Shader* shader)
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    Camera& camera = Camera::Intance();
    RenderContext rc;
    rc.view = camera.GetView();
    rc.projection = camera.getProjection();

    shader->Begin(dc, rc);
    shader->Draw(dc, this->model.get());
    shader->End(dc);
}

void DebugObject::DrawImGUi()
{
    if (ImGui::Begin("TestObject"))
    {
        ImGui::InputFloat3("position", &this->position.x);
        ImGui::InputFloat3("angle", &this->angle.x);
        ImGui::InputFloat3("scale", &this->scale.x);
    }
    ImGui::End();
}

void DebugObject::UpdateTransform()
{
    // �X�P�[���s����쐬
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    //��]�s����쐬
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    // �ʒu�s����쐬
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    // 3�̍s���g�ݍ��킹�A���[���h�s����쐬
    DirectX::XMMATRIX W = S * R * T;
    // �v�Z�������[���h�s������o��
    DirectX::XMStoreFloat4x4(&transform, W);
}