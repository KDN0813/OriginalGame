#pragma once
#include "System/ClassBase/Singleton.h"
#include "Object/Object.h"

class Transform3DComponent;

class GameObject : public Singleton<GameObject>
{
public:
    GameObject();
    ~GameObject() {};

    // �X�V����
    // ��ɍ폜���ꂽ�G�l�~�[�����X�g�����������
    void Update();

    // �e��ݒ�E�擾�֐�
    void SetPlayer(std::shared_ptr<Object> player) { this->player_Wptr = player; };
    void SetStageFloor(std::shared_ptr<Object> stage) { this->stage_foor_Wptr = stage; };
    void SetStageWall(std::shared_ptr<Object> stage) { this->stage_wall_Wptr = stage; };
    void SetEnemy(std::shared_ptr<Object> enenmy) { this->enemy_Wptr_pool.emplace_back(enenmy); };
    std::shared_ptr<Object> GetPlayer() const { return this->player_Wptr.lock(); }
    std::shared_ptr<Object> GetStageFoor() const { return this->stage_foor_Wptr.lock(); }
    std::shared_ptr<Object> GetStageWall() const { return this->stage_wall_Wptr.lock(); }
    const std::vector<std::weak_ptr<Object>>& GetEnemyWptPool() const { return this->enemy_Wptr_pool; };
    
    // �G�l�~�[�̑S�폜
    void EnemyAllClear();
    // �ǉ��œG�𐶐�����
    void AddCreateEnemy(float elapsed_time,ObjectManager& object_manager);
private:
    std::weak_ptr<Object> player_Wptr;
    std::weak_ptr<Transform3DComponent> player_transform_Wptr;
    std::weak_ptr<Object> stage_foor_Wptr;
    std::weak_ptr<Object> stage_wall_Wptr;
    std::vector<std::weak_ptr<Object>> enemy_Wptr_pool;

    float create_enemy_cool_time = 0.0f;    // �G�����̃N�[���^�C��
#ifdef _DEBUG
public:
    void DebugDrawGUI();

#endif // DEBUG
};