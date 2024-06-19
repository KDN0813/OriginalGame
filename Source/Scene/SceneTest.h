#pragma once

#include "Scene/Scene.h"

class SceneTest : public Scene
{
public:
    SceneTest() {}
    ~SceneTest() override {};

    void Initialize() override;
    void Finalize() override;
    void Update(float elapsed_time) override;
    void Render() override;
};

