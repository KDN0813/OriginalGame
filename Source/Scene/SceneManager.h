#pragma once

#include "System/ClassBase/Singleton.h"
#include "Scene.h"

#include "Object\Object.h"
#include "Shader\SpriteShader.h"

class SceneManager : public Singleton<SceneManager>
{
public:
    SceneManager() :Singleton(this) {};
    ~SceneManager(){}

    void Update(float elpsed_time);

    void Render();

    void Clear();

    void ChangeScene(Scene* scene);

private:
    Scene* current_scene = nullptr;
    Scene* next_scene    = nullptr;

    // ポーズ画面用オブジェクト
    std::unique_ptr<SpriteShader> sprite_shader;
    ObjectManager object_manager;
#ifdef _DEBUG
public:
    void DrawDebugGUI();

#endif // _DEBUG
};

