#pragma once

#include "System/ClassBase/Singleton.h"
#include "Scene.h"

class SceneManager : public Singleton<SceneManager>
{
public:
    SceneManager() :Singleton(this) {}
    ~SceneManager(){}

    void Update(float elpsed_time);

    void Render();

    void Clear();

    void ChangeScene(Scene* scene);

private:
    Scene* current_scene = nullptr;
    Scene* next_scene    = nullptr;

#ifdef _DEBUG
public:
    void DrawDebugGUI();

#endif // _DEBUG
};

