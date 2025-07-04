#pragma once

#include "Scene/Scene.h"
#include <vector>
#include <memory>
#include "Object/Object.h"
#include "Object/GameObjectRegistry.h"
#include "Collision/CircleCollisionManager.h"
#include "GridObjectManager/GridObjectManager.h"

// シェーダー
#include "Shader/InstanceModelShader.h"
#include "Shader/ModelShader.h"
#include "Shader/ParticleSystem.h"
#include "Shader\LightManager.h"

class SceneGame : public Scene 
{
public:
    enum class CHANGE_SCENE_STATE
    {
        START = 0,
        RUN,
        END,
    };
public:
    SceneGame() {};
    ~SceneGame() override {};

	void Initialize() override;
	void Finalize() override;
	void Update(float elapsed_time) override;
	void Render() override;
    void ReStart() override;
private:
    // ゲーム状態を処理する
    // GameStatusに応じてシーンの遷移・リスタート処理を行う
    void ProcessGameState();

private:
    // シェーダー
    std::unique_ptr<InstancingModelShader> instancing_model_shader;
    std::unique_ptr<ModelShader> model_shader;

    std::shared_ptr<ObjectManager> object_manager;
    GameObjectRegistry game_object;
    CircleCollisionManager circle_collision_manager;
    ParticleSystem particle_system;
    LightManager light_manager;
    GridObjectManager grid_object_manager;

    CHANGE_SCENE_STATE change_state = CHANGE_SCENE_STATE::START;    // シーン遷移用ステート
#ifdef _DEBUG
    std::shared_ptr<Object> debug_Particle;
#endif // _DEBUG
#ifdef _DEBUG
    void DebugDrawGUI();

    void DrawShaderImGui();
#endif // _DEBUG
};
