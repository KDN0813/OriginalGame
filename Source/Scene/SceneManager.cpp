#include "SceneManager.h"
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
        if (ImGui::Button("Chenge SceneTitle"))
        {
            ChangeScene(new SceneTitle);
        }
        if (ImGui::Button("Chenge SceneGame"))
        {
            ChangeScene(new SceneGame);
        }
        if (ImGui::Button("Chenge SceneResult"))
        {
            ChangeScene(new SceneResult);
        }
    }
    ImGui::End();
}

#endif // DEBUG