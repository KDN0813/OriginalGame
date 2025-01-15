#pragma once
#include <vector>
#include <memory>
#include "System/ClassBase/Singleton.h"

class CircleCollisionComponent;

class CircleCollisionManager : public Singleton<CircleCollisionManager>
{
public:
    CircleCollisionManager():Singleton(this){}
    ~CircleCollisionManager() {};

    // �Փ˔�����s��
    void EvaluateCollision(const std::shared_ptr<CircleCollisionComponent>& collison);

    // �G�l�~�[�Ɠ����蔻����s��
    void VsEnemy();
    // �v���C���[�Ɠ����蔻����s��
    void VsPlayer();
private:
    std::vector<std::weak_ptr<CircleCollisionComponent>> vs_enemy_collision_Wptr_pool;  // �G�l�~�[�Ɣ�����s���R���W����
    std::vector<std::weak_ptr<CircleCollisionComponent>> vs_player_collision_Wptr_pool; // �v���C���[�Ɣ�����s���R���W����
};

