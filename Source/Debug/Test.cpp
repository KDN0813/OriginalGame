#include "Test.h"
#include <filesystem>
#include <fstream>
#include <imgui.h>

CerealTest::CerealTest()
    :player_data()
{
}

void CerealTest::Serialize(const char* creal_file_name)
{
    std::ofstream ofs(creal_file_name, std::ios::binary);
    // �V���A���C�Y�A�E�g�v�b�g
    cereal::BinaryOutputArchive serialization(ofs);
    serialization(this->player_data);
}

void CerealTest::Deserialize(const char* creal_file_name)
{
    if (std::filesystem::exists(creal_file_name))
    {
        // ���݂���ꍇ(�o�C�i���t�@�C���ǂݍ���)
        std::ifstream ifs(creal_file_name, std::ios::binary);
        //// �V���A���C�Y�C���v�b�g
        cereal::BinaryInputArchive deserialization(ifs);
        deserialization(this->player_data);
    }
}

void CerealTest::DrawImGui()
{
    const char* creal_file_name = "test.mae";

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
            ImGui::InputFloat3("pos", &this->player_data.pos.x);
            ImGui::InputInt("hp", &this->player_data.hp);
            ImGui::TreePop();
        }
    }
    ImGui::End();
}
