#include "GameData.h"
#ifdef _DEBUG
#include <magic_enum.hpp>
#include "Debug/ImGuiHelper.h"
#endif // DEBUG

GameData::GameData()
    :Singleton(this)
{
#ifdef _DEBUG
    for (int i = 0; i < static_cast<int>(GameStatus::MAX); ++i)
    {
        this->game_status_name_pool.emplace_back(
            magic_enum::enum_name(static_cast<GameStatus>(i)));
    }
#endif // _DEBUG
}

#ifdef _DEBUG

void GameData::DebugDrawGUI()
{
    if (ImGui::Begin("GameData"))
    {
        ImGui::InputInt("Score##GameData", &this->param.score);
        ImGui::InputFloat("Game Elapsed Time##GameData", &this->param.game_elapsed_time);

        // �Q�[����Ԃ̕\��
        {
            {
                std::string label, current_game_state_name{};
                label = "GameStatus##GameData";
                current_game_state_name = magic_enum::enum_name(this->param.game_status);

                if (ImGui::ComboUI(label, current_game_state_name, this->game_status_name_pool, this->select_game_status_index))
                {
                    this->param.game_status = static_cast<GameStatus>(this->select_game_status_index);
                }
            }
        }

        if (ImGui::Button("ClearParam##GameData"))
        {
            ClearParam();
        }
    }
    ImGui::End();
}

#endif // _DEBUG