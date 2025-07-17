#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include <vector>
#ifdef _DEBUG
#include "Debug\DebugPrimitiveRenderer.h"
#endif // _DEBUG

class Object;
class ObjectManager;
class Transform3DComponent;

struct EnemyConfig;

// �G�l�~�[�̐������s���R���|�[�l���g
class EnemySpawnerComponent : public Component
{
public:
    enum EyemyType
    {
        Normal = 0,
        Max,
    };

    struct Param
    {
        float create_cool_time_max; // �����̃N�[���^�C��(�ő�l)
        float create_cool_time_min; // �����̃N�[���^�C��(�Œ�l)
        float create_cool_timer;    // �����̃N�[���^�C��

        float min_spawn_dist;   // �����͈͂̍ŏ�
        float max_spawn_dist;   // �����͈͂̍ŏ�

        int min_enemies_per_spawn;  // ��x�ɐ�������G�̍Œ�l
        int max_enemies_per_spawn;  // ��x�ɐ�������G�̍ő�l

        float spawn_offset_interval = 2.0f; // �e�X�|�[���n�_�̂΂炯��
    };

public:
    EnemySpawnerComponent(Param param);
    ~EnemySpawnerComponent() {};

    // �J�n�֐�
    void Start()  override;
    // �I���֐�
    void End()  override {};
    // ���X�^�[�g����
    void ReStart() override { this->param = this->default_param; };      // �p�����[�^�̏�����
    // �X�V�֐�
    void Update(float elapsed_time) override;

    // ���O�擾
    const char* GetName()const  override { return "EnemySpawner"; };
    // �D��x
    const PRIORITY GetPriority()const noexcept  override { return PRIORITY::DEFAULT; };

    void SetChainScoreCounterObject(const std::shared_ptr<Object>& object) { this->chain_score_counter_object_Wptr = object; };
    void SetObjectManager(const std::shared_ptr<ObjectManager>& manager) { this->object_manager_Wptr = manager; }
private:
    void UpdateEnemySpawner(const std::shared_ptr<ObjectManager>& manager);
    void CreateEnemy(const std::shared_ptr<ObjectManager>& manager, const EnemyConfig& config);
    
private:
    std::weak_ptr<ObjectManager> object_manager_Wptr;
    Param param;
    Param default_param;

    std::vector<EnemyConfig> enemy_config_pool;

private:
    std::weak_ptr<Object> player_Wptr;
    std::weak_ptr<Transform3DComponent> player_transform_Wptr;
    std::weak_ptr<Object> stage_floor_Wptr;
    std::weak_ptr<Object> stage_wall_Wptr;
    std::vector<std::weak_ptr<Object>> enemy_Wptr_pool;

    std::weak_ptr<Object> chain_score_counter_object_Wptr;
#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI()  override;
    /**
     * �f�o�b�N�̏���3D��ʏ�ɏo�͂���֐�
     */
    void DrawDebugPrimitive()  override;
    /**
     * �f�o�b�O�v���~�e�B�u�\���pImGui
     */
    void DrawDebugPrimitiveGUI()  override;
    bool IsDebugPrimitive() override { return true; }   // DebugPrimitive�����݂��邩
private:
    CylinderParam min_spawn_area;
    CylinderParam max_spawn_area;
    CylinderParam spawn_area;
    int debug_spawn_count = 0;      // ImGui�œG�𐶐����鎞�̐�
#endif // DEBUG
};

