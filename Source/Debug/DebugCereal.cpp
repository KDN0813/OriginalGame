#include "DebugCereal.h"
#include <filesystem>
#include <fstream>
#include <imgui.h>

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>

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
void DebugCereal::PlayerData::serialize(T& archive)
{
    archive(
        CEREAL_NVP(name),
        CEREAL_NVP(pos),
        CEREAL_NVP(hp)
    );
}

#pragma endregion serialize


DebugCereal::DebugCereal()
    :player_data()
{
}

void DebugCereal::Serialize(const char* creal_file_name)
{
    std::ofstream ofs(creal_file_name, std::ios::binary);
    // シリアライズアウトプット
    cereal::BinaryOutputArchive serialization(ofs);
    serialization(this->player_data);
}

void DebugCereal::Deserialize(const char* creal_file_name)
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

void DebugCereal::DrawImGui()
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