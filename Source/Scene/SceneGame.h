#pragma once

#include "Scene/Scene.h"
#include <vector>
#include <memory>
#include "Object/Object.h"
#include "Object/GameObject.h"
#include "Collision/CollisionManager.h"

#include "Sprite.h"

// シェーダー
#include "Shader/InstanceModelShader.h"
#include "Shader/ModelShader.h"

class SceneGame : public Scene 
{
public:
    SceneGame() {};
    ~SceneGame() override {};

	void Initialize() override;
	void Finalize() override;
	void Update(float elapsed_time) override;
	void Render() override;
    void ReStart();
private:
    // シェーダー
    std::unique_ptr<InstancingModelShader> instancing_model_shader;
    std::unique_ptr<ModelShader> model_shader;

    GameObject game_object;
    ObjectManager object_manager;
    CollisionManager collision_manager;
#ifdef _DEBUG
    enum class ImGuiMode
    {
        Object = 0,
        Shader,
        Camera,
        Collison,
        System,
    };
    ImGuiMode mode_index = ImGuiMode::Object;
    std::unique_ptr<Sprite> sprite;

    void DrawImGui();

    void DrawShaderImGui();
    void DrawSystemImGui();
#endif // _DEBUG
};
