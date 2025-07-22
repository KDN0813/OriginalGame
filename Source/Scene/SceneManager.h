#pragma once

#include "System/ClassBase/Singleton.h"
#include "Scene.h"

#include "Object\Object.h"
#include "Shader\SpriteShader.h"

class SpriteFadeControllerComponent;

class SceneManager : public Singleton<SceneManager>
{
public:
    SceneManager() :Singleton(this) {};
    ~SceneManager(){}

    void Initialize();

    void Update(float elpsed_time);

    void Render();

    void Clear();

    void ChangeScene(Scene* scene);

    const std::shared_ptr<Object>& GetPauseObject()const { return this->pause_object; }
    const std::shared_ptr<Object>& GetFadeObject()const { return this->fade_object; }
    const std::shared_ptr<SpriteFadeControllerComponent> GetFadeControlle() { return this->fade_controller_Wptr.lock(); }
private:
    Scene* current_scene = nullptr;
    Scene* next_scene    = nullptr;

    // ポーズ画面用オブジェクト
    std::shared_ptr<Object> pause_object;   // ポーズ用オブジェクト
    std::shared_ptr<Object> fade_object;    // フェード用オブジェクト
    std::weak_ptr<SpriteFadeControllerComponent> fade_controller_Wptr;    // フェード用オブジェクトのフェードコントローラ
    ObjectManager common_object_manager;
#ifdef _DEBUG
public:
    void DrawDebugGUI();

#endif // _DEBUG
};

