#pragma once
#include "System/ClassBase/Singleton.h"
#include "Object/Object.h"

class Transform3DComponent;

// �Q�[�����̏d�v�ȃI�u�W�F�N�g��o�^�E�Q�Ƃ��邽�߂̃N���X
class GameObjectRegistry : public Singleton<GameObjectRegistry>
{
public:
    GameObjectRegistry();
    ~GameObjectRegistry() {};

    // �X�V����
    // ��ɍ폜���ꂽ�G�l�~�[�����X�g�����������
    void Update();

    // �e��ݒ�E�擾�֐�
    void SetPlayer(std::shared_ptr<Object> player) { this->player_Wptr = player; };
    void SetStageFloor(std::shared_ptr<Object> stage) { this->stage_floor_Wptr = stage; };
    void SetStageWall(std::shared_ptr<Object> stage) { this->stage_wall_Wptr = stage; };
    void SetEnemy(std::shared_ptr<Object> enemy) { this->enemy_Wptr_pool.emplace_back(enemy); };
    std::shared_ptr<Object> GetPlayer() const { return this->player_Wptr.lock(); }
    std::shared_ptr<Object> GetStageFloor() const { return this->stage_floor_Wptr.lock(); }
    std::shared_ptr<Object> GetStageWall() const { return this->stage_wall_Wptr.lock(); }
    const std::vector<std::weak_ptr<Object>>& GetEnemyWptPool() const { return this->enemy_Wptr_pool; };
    
    // �G�l�~�[�̑S�폜
    void EnemyAllClear();
private:
    std::weak_ptr<Object> player_Wptr;
    std::weak_ptr<Transform3DComponent> player_transform_Wptr;
    std::weak_ptr<Object> stage_floor_Wptr;
    std::weak_ptr<Object> stage_wall_Wptr;
    std::vector<std::weak_ptr<Object>> enemy_Wptr_pool;

#ifdef _DEBUG
public:
    void DebugDrawGUI();

#endif // DEBUG
};