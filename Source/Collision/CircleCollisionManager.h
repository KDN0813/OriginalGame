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

    // 衝突判定を行う
    void EvaluateCollision(const std::shared_ptr<CircleCollisionComponent>& collison);

    // エネミーと当たり判定を行う
    void VsEnemy();
private:
    std::vector<std::weak_ptr<CircleCollisionComponent>> vs_enemy_collision_Wptr_pool; // エネミーと判定を行うコリジョン
};

