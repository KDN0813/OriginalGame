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
#ifdef _DEBUG
    if (ImGui::Begin("DebugObject"))
    {
        ImGui::DragFloat3("position", &this->position.x, 0.01f);
        ImGui::DragFloat3("angle", &this->angle.x, 0.05f);
        float scale_dummy = this->scale.x;
        ImGui::DragFloat("scale", &scale_dummy, 0.0001f);
        if (this->scale.x != scale_dummy)
        {
            this->scale.x = this->scale.y = this->scale.z = scale_dummy;
        }
    }
    ImGui::End();
#endif _DEBUG
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


void DebugPlayer::Update(float elapsedTime)
{


    DebugObject::Update(elapsedTime);
}
