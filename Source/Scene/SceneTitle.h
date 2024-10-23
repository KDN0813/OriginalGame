#pragma once
#include "memory"

#include "Scene.h"

#include "Shader/SpriteShader.h"
#include "Object/Object.h"

class SceneTitle : public Scene
{
public:
    SceneTitle(){}
    ~SceneTitle() override{};

    void Initialize() override;
    void Finalize() override;
    void Update(float elapsed_time) override;
    void Render() override;
    void ReStart() override;

private:
    std::unique_ptr<SpriteShader> sprite_shader;
    ObjectManager object_manager;

#ifdef _DEBUG
public:
    void DebugDrawGUI() override;
#endif // DEBUG
};