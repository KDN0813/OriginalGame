#pragma once

#include "Scene/Scene.h"
#include <vector>
#include <memory>
#include "Debug/DebugCereal.h"
#include "Object/Object.h"

// シェーダー
#include "Shader/InstanceModelShader.h"
#include "Shader/ModelShader.h"
#include "Shader/ParticleSystem.h"

class SceneDebug : public Scene
{
public:
    SceneDebug();
    ~SceneDebug() override {};

    void Initialize() override;
    void Finalize() override;
    void Update(float elapsed_time) override;
    void Render() override;

private:

    // シェーダー
    std::unique_ptr<ModelShader> model_shader;

    ObjectManager object_manager;
    ParticleSystem particle_system;

#ifdef _DEBUG
    void DebugDrawGUI() override;

    void DrawShaderImGui();
#endif // _DEBUG
};

