#include "SceneManager.h"

void SceneManager::Update(float elpsed_time)
{
    if (next_scene != nullptr)
    {
        Clear();

        current_scene = next_scene;
        next_scene = nullptr;

        if (!current_scene->IsReady())
        {
            current_scene->Initialize();
        }
    }

    if (current_scene != nullptr)
    {
        current_scene->Update(elpsed_time);
    }
}

void SceneManager::Render()
{
    if (current_scene != nullptr)
    {
        current_scene->Render();
    }
}

void SceneManager::Clear()
{
    if (current_scene != nullptr)
    {
        current_scene->Finalize();
        delete current_scene;
        current_scene = nullptr;
    }
}

void SceneManager::ChangeScene(Scene* scene)
{
    next_scene = scene;
}
