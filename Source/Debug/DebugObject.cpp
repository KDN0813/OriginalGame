#include "DebugObject.h"
#include <imgui.h>

#include "System/Misc.h"
#include "Graphics/Graphics.h"
#include "Camera/Camera.h"

DebugObject::DebugObject(const char* filename, DirectX::XMFLOAT3 position)
    :model(std::make_unique<Model>(filename))
    , position(position)
    , angle()
    , scale({ 1.0f,1.0f ,1.0f })
    , transform()
{
}

void DebugObject::Update(float elapsed_time)
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


void DebugPlayer::Update(float elapsedTime)
{


    DebugObject::Update(elapsedTime);
}




ObjectManager::ObjectManager(const char* filename)
{
    // オブジェクトの生成
    {
        float offset = 3.0f;
        for (int x = 0; x < 1; ++x)
        {
            for (int z = 0; z < 1; ++z)
            {
                DirectX::XMFLOAT3 pos =
                {
                    static_cast<float>(x) * offset,
                    0.0f,
                    static_cast<float>(z) * offset,
                };
                objects.emplace_back(std::make_unique<DebugObject>(filename, pos));
                inputData.emplace_back();
            }
        }
    }

    // インスタンスごとの行列を保持するバッファ作成
    {
        D3D11_BUFFER_DESC buffer_desc = {};
        D3D11_SUBRESOURCE_DATA subresource_data = {};

        buffer_desc.ByteWidth = static_cast<UINT>(sizeof(DirectX::XMFLOAT4X4) * this->OBJECT_MAX);
        buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
        buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        buffer_desc.CPUAccessFlags = 0;
        buffer_desc.MiscFlags = 0;
        buffer_desc.StructureByteStride = 0;

        subresource_data.pSysMem = inputData.data();
        subresource_data.SysMemPitch = 0;
        subresource_data.SysMemSlicePitch = 0;

        Graphics& graphics = Graphics::Instance();
        ID3D11Device* device = graphics.GetDevice();
        HRESULT hr = device->CreateBuffer(&buffer_desc, &subresource_data, inputBuffer.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    }
}

void ObjectManager::Update(float elapsed_time)
{
    for (auto& obj : objects)
    {
        obj->Update(elapsed_time);
    }
}

void ObjectManager::DrawImGUi()
{
    this->objects[0]->DrawImGUi();
}
