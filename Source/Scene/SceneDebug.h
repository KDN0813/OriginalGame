#pragma once

#include "Scene/Scene.h"
#include <vector>
#include <memory>
#include "Camera/CameraController.h"
#include "Debug/DebugCereal.h"
#include "Debug/DebugObject.h"
#include "Model/InstancingModel.h"
#include "Object/Object.h"

// シェーダー
#include "Shader/InstanceModelShader.h"

class SceneDebug : public Scene
{
public:
    SceneDebug();
    ~SceneDebug() override {};

    void Initialize() override;
    void Finalize() override;
    void Update(float elapsed_time) override;
    void Render() override;

#ifdef _DEBUG
    enum class ImGuiMode
    {
        Object = 0,
        System,
    };
    ImGuiMode mode_index = ImGuiMode::Object;
    void DrawImGui();
#endif // _DEBUG
private:
    CameraController cameraController;

    // シェーダー
    std::unique_ptr<InstancingModelShader> instance_model_shader;

    ObjectManager object_manager;
};

