#pragma once

#include "System/ClassBase/Singleton.h"
#include "Scene.h"

class SceneManager : public Singleton<SceneManager>
{
public:
    SceneManager() :Singleton(this) {}
    ~SceneManager(){}

    void Update(float elpsedTime);

    void Render();

    void Clear();

    void ChangeScene(Scene* scene);

private:
    Scene* currentScene = nullptr;
    Scene* nextScene    = nullptr;
};

