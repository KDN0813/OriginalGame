#pragma once

#include "Scene/Scene.h"
#include <vector>
#include <memory>
#include "Object/Object.h"
#include "Object/GameObject.h"
#include "Collision/CircleCollisionManager.h"
#include "GridObjectManager/GridObjectManager.h"

// �V�F�[�_�[
#include "Shader/InstanceModelShader.h"
#include "Shader/ModelShader.h"
#include "Shader/ParticleSystem.h"
#include "Shader\LightManager.h"
#include "Shader\SkyBox.h"

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
    // �Q�[����Ԃ���������
    // GameStatus�ɉ����ăV�[���̑J�ځE���X�^�[�g�������s��
    void ProcessGameState();

private:
    // �V�F�[�_�[
    std::unique_ptr<InstancingModelShader> instancing_model_shader;
    std::unique_ptr<ModelShader> model_shader;
    std::unique_ptr<SkyBox> sky_box;

    GameObject game_object;
    ObjectManager object_manager;
    CircleCollisionManager circle_collision_manager;
    ParticleSystem particle_system;
    LightManager light_manager;
    GridObjectManager grid_object_manager;

    CHANGE_SCENE_STATE change_state = CHANGE_SCENE_STATE::START;    // �V�[���J�ڗp�X�e�[�g
#ifdef _DEBUG
    const size_t enemy_max = 500;
    std::shared_ptr<Object> debug_Particle;
#else
    const int enemy_max = 5000;
#endif // _DEBUG
#ifdef _DEBUG
    void DebugDrawGUI();

    void DrawShaderImGui();
#endif // _DEBUG
};
