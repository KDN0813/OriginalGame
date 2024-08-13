#pragma once

#include "Scene/Scene.h"
#include <vector>
#include <memory>
#include "Camera/CameraController.h"
#include "Debug/DebugCereal.h"
#include "Debug/DebugObject.h"
#include "Model/InstancingModel.h"
#include "GameObject/GameObject.h"

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
    void DrawImGui();
#endif // _DEBUG
private:
    CameraController cameraController;
    DebugObject stage;
    std::vector<std::unique_ptr<DebugObject>> models;
    std::unique_ptr<InstancingModel> instancing_model;

    std::shared_ptr<GameObject> debug_object;
};

