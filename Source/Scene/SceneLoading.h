    #pragma once

#include "Scene.h"
#include <memory>
#include <thread>

#include "Object/Object.h"

class SceneLoading : public Scene
{
public:
    SceneLoading(Scene* next_scene) : next_scene(next_scene) {}
    ~SceneLoading(){}

    void Initialize() override;
    void Finalize() override;
    void Update(float elapsed_time) override;
    void Render() override;

private:
    static void LoadingThread(SceneLoading* scene);

private:
    ObjectManager object_manager;

private:
    Scene* next_scene   = nullptr;
    std::unique_ptr<std::thread> thread;

#ifdef _DEBUG
public:
    void DebugDrawGUI()override {};

#endif // DEBUG
};