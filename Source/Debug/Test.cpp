#include "Test.h"
#include <filesystem>
#include <fstream>
#include <imgui.h>

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>

#include "Graphics/Graphics.h"
#include "Camera/Camera.h"

#pragma region serialize

namespace DirectX
{
    template<class T>
    void serialize(T& archive, DirectX::XMFLOAT3& v)
    {
        archive(
            cereal::make_nvp("x", v.x),
            cereal::make_nvp("y", v.y),
            cereal::make_nvp("z", v.z)
        );
    };
}

template<class T>
void CerealTest::PlayerData::serialize(T& archive)
{
    archive(
        CEREAL_NVP(name),
        CEREAL_NVP(pos),
        CEREAL_NVP(hp)
    );
}

#pragma endregion serialize


CerealTest::CerealTest()
    :player_data()
{
}

void CerealTest::Serialize(const char* creal_file_name)
{
    std::ofstream ofs(creal_file_name, std::ios::binary);
    // シリアライズアウトプット
    cereal::BinaryOutputArchive serialization(ofs);
    serialization(this->player_data);
}

void CerealTest::Deserialize(const char* creal_file_name)
{
    if (std::filesystem::exists(creal_file_name))
    {
        // 存在する場合(バイナリファイル読み込む)
        std::ifstream ifs(creal_file_name, std::ios::binary);
        //// シリアライズインプット
        cereal::BinaryInputArchive archive(ifs);
        archive(
            CEREAL_NVP(this->player_data)
        );
    }
}

void CerealTest::DrawImGui()
{
    const char* creal_file_name = "DebugData/test.mae";

    if (ImGui::Begin("CerealTest"))
    {
        if (ImGui::Button("Serialize"))
        {
            this->Serialize(creal_file_name);
        }
        if (ImGui::Button("Deserialize"))
        {
            this->Deserialize(creal_file_name);
        }

        if (ImGui::TreeNode("PlayerData"))
        {
            ImGui::Text(this->player_data.name.c_str());
            ImGui::InputFloat3("pos", &this->player_data.pos.x);
            ImGui::InputInt("hp", &this->player_data.hp);

            if (ImGui::Button("ChangeName"))
            {
                this->player_data.name = "maeyama";
            }
            if (ImGui::Button("reset"))
            {
                this->player_data.name = "";
                this->player_data.hp = 0;
                this->player_data.pos = {};
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();
}

TestObject::TestObject(const char* filename)
    :model(std::make_unique<Model>(filename))
    , position()
    , angle()
    , scale({ 0.01f,0.01f ,0.01f })
    , transform()
{
}

void TestObject::Update(float elapsedTime)
{
    this->UpdateTransform();

    this->model->UpdateTransform(this->transform);
}

void TestObject::Render(Shader* shader)
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

void TestObject::DrawImGUi()
{
    if (ImGui::Begin("TestObject"))
    {
        ImGui::InputFloat3("position", &this->position.x);
        ImGui::InputFloat3("angle", &this->angle.x);
        ImGui::InputFloat3("scale", &this->scale.x);
    }
    ImGui::End();
}

void TestObject::UpdateTransform()
{
    // スケール行列を作成
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    //回転行列を作成
    DirectX::XMMATRIX R= DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    // 位置行列を作成
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    // 3つの行列を組み合わせ、ワールド行列を作成
    DirectX::XMMATRIX W = S * R * T;
    // 計算したワールド行列を取り出す
    DirectX::XMStoreFloat4x4(&transform, W);
}
