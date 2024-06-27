#pragma once

#include "Scene/Scene.h"
#include "Debug/DebugCereal.h"
#include "Debug/DebugObject.h"

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
    DebugObject player;
};

