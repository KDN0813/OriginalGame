#pragma once
#include "Component.h"
#include <vector>

class Object;
class ObjectManager;
class Transform3DComponent;

// �G�l�~�[�̐������s���R���|�[�l���g
class EnemySpawnerComponent : public Component
{
public:
    struct Param
    {
        float create_cool_time_max; // �����̃N�[���^�C��(�ő�l)
        float create_cool_time_min; // �����̃N�[���^�C��(�Œ�l)
        float create_cool_timer;    // �����̃N�[���^�C��
    };

public:
    EnemySpawnerComponent(Param param);
    ~EnemySpawnerComponent() {};

    // �J�n�֐�
    void Start()  override {};
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

    void SetObjectManager(const std::shared_ptr<ObjectManager>& manager) { this->object_manager_Wptr = manager; }
private:
    void AddCreateEnemy(const std::shared_ptr<ObjectManager>& manager);
    
private:
    std::weak_ptr<ObjectManager> object_manager_Wptr;
    Param param;
    Param default_param;

private:
    std::weak_ptr<Object> player_Wptr;
    std::weak_ptr<Transform3DComponent> player_transform_Wptr;
    std::weak_ptr<Object> stage_floor_Wptr;
    std::weak_ptr<Object> stage_wall_Wptr;
    std::vector<std::weak_ptr<Object>> enemy_Wptr_pool;

#ifdef _DEBUG
public:
    /**
     * �f�o�b�N�̏���2D��ʂɏo�͂���֐�
     */
    void DrawDebugGUI()  override;
    /**
     * �f�o�b�N�̏���3D��ʏ�ɏo�͂���֐�
     */
    void DrawDebugPrimitive()  override {};
    /**
     * �f�o�b�O�v���~�e�B�u�\���pImGui
     */
    void DrawDebugPrimitiveGUI()  override {};
    bool IsDebugPrimitive() override { return false; }   // DebugPrimitive�����݂��邩
#endif // DEBUG
};

