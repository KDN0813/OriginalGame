#pragma once

#include "Scene/Scene.h"
#include "Debug/Test.h"

class SceneTest : public Scene
{
public:
    SceneTest() {};
    ~SceneTest() override {};

    void Initialize() override;
    void Finalize() override;
    void Update(float elapsed_time) override;
    void Render() override;

#ifdef _DEBUG
    void DrawImGui();
#endif // _DEBUG
private:
    CerealTest cerealTest;
};

