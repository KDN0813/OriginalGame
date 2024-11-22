#pragma once

#include "Scene/Scene.h"
#include <vector>
#include <memory>
#include "Object/Object.h"
#include "Object/GameObject.h"
#include "Collision/CircleCollisionManager.h"

// シェーダー
#include "Shader/InstanceModelShader.h"
#include "Shader/ModelShader.h"
#include "Shader/SpriteShader.h"
#include "Shader/ParticleSystem.h"
#include "Shader\LightManager.h"

class SceneGame : public Scene 
{
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

    // プレイヤーとエネミーの当たり判定
    void PlayerVsEnemy();
private:
    // シェーダー
    std::unique_ptr<InstancingModelShader> instancing_model_shader;
    std::unique_ptr<ModelShader> model_shader;
    std::unique_ptr<SpriteShader> sprite_shader;

    GameObject game_object;
    ObjectManager object_manager;
    CircleCollisionManager circle_collision_manager;
    ParticleSystem particle_system;
    LightManager light_manager;
#ifdef _DEBUG
    void DebugDrawGUI();

    void DrawShaderImGui();
#endif // _DEBUG
};
