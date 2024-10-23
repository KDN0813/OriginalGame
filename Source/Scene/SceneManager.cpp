#include "SceneManager.h"
#ifdef _DEBUG
#include <imgui.h>
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
    this->current_scene->DebugDrawGUI();
    if (ImGui::BeginTabItem("Scene"))
    {
        ImGui::EndTabItem();
    }
}

#endif // DEBUG