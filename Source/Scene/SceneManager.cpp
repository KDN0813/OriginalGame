#include "SceneManager.h"
#include "System/GameData.h"

#ifdef _DEBUG
#include <imgui.h>
#include "Scene/SceneTitle.h"
#include "Scene/SceneResult.h"
#include "Scene/SceneGame.h"
#include "Scene/SceneLoading.h"
#endif // DEBUG


void SceneManager::Update(float elpsed_time)
{
    if (this->next_scene != nullptr)
    {
        Clear();

        this->current_scene = this->next_scene;
        this->next_scene = nullptr;

        if (!this->current_scene->IsReady())
        {
            // ゲーム状態をデフォルトに設定
            GameData* game_data = GameData::Instance();
            if (game_data)
            {
                game_data->SetGameStatus(GameData::GameStatus::DEFAULT);
            }

            this->current_scene->Initialize();
        }
    }

    if (this->current_scene != nullptr)
    {
        this->current_scene->Update(elpsed_time);
    }
}

void SceneManager::Render()
{
    if (this->current_scene != nullptr)
    {
        this->current_scene->Render();
    }
}

void SceneManager::Clear()
{
    if (this->current_scene != nullptr)
    {
        this->current_scene->Finalize();
        delete this->current_scene;
        this->current_scene = nullptr;
    }
}

void SceneManager::ChangeScene(Scene* scene)
{
    this->next_scene = scene;
}

#ifdef _DEBUG

void SceneManager::DrawDebugGUI()
{
    // シーン毎のデバッグ処理
    this->current_scene->DebugDrawGUI();
    
    if (ImGui::Begin("Scene Manager"))
    {
        // ロードシーンでないなら表示する
        if (dynamic_cast<SceneLoading*>(this->current_scene) == nullptr)
        {
            // シーンのリセット
            if (ImGui::Button("ReStart"))
            {
                this->current_scene->ReStart();
            }
            // 直接シーンを切り替える
            if(ImGui::CollapsingHeader("Chenge Scene", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (dynamic_cast<SceneTitle*>(this->current_scene) == nullptr &&
                    ImGui::Button("SceneTitle"))
                {
                    ChangeScene(new SceneTitle);
                }
                if (dynamic_cast<SceneGame*>(this->current_scene) == nullptr &&
                    ImGui::Button("SceneGame"))
                {
                    ChangeScene(new SceneGame);
                }
                if (dynamic_cast<SceneResult*>(this->current_scene) == nullptr &&
                    ImGui::Button("SceneResult"))
                {
                    ChangeScene(new SceneResult);
                }
            }
            // ロードを挟んで切り替える
            if (ImGui::CollapsingHeader("Chenge Load cene", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (dynamic_cast<SceneTitle*>(this->current_scene) == nullptr &&
                    ImGui::Button("SceneTitle##Load"))
                {
                    ChangeScene(new SceneLoading(new SceneTitle));
                }
                if (dynamic_cast<SceneGame*>(this->current_scene) == nullptr &&
                    ImGui::Button("SceneGame##Load"))
                {
                    ChangeScene(new SceneLoading(new SceneGame));
                }
                if (dynamic_cast<SceneResult*>(this->current_scene) == nullptr &&
                    ImGui::Button("SceneResult##Load"))
                {
                    ChangeScene(new SceneLoading(new SceneResult));
                }
            }
        }
    }
    ImGui::End();
}

#endif // DEBUG