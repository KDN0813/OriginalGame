#pragma once
#include <memory>
#include "Scene.h"
#include "Object/Object.h"

// シェーダー
#include "Shader/SpriteShader.h"

class SceneResult : public Scene
{
public:
    SceneResult() {}
    ~SceneResult() override {};

    void Initialize() override;
    void Finalize() override;
    void Update(float elapsed_time) override;
    void Render() override;

private:
    std::unique_ptr<SpriteShader> sprite_shader;
    ObjectManager object_manager;

#ifdef _DEBUG
public:
    void DebugDrawGUI()override {};

#endif // DEBUG
};

