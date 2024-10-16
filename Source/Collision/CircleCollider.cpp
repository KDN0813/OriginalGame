#include "CircleCollider.h"
#include "Collision/Collision.h"

#include "Component/CircleComponent.h"

void CircleCollider::Update()
{
    CheckCollision();
}

void CircleCollider::AddCircle(std::shared_ptr<CircleComponent>& circle)
{
    if (!circle) return;
    
    if (circle->GetCollisionType() & COLLISION_TYPE::COLLISION_TYPE_ATTACKER)
    {
        this->circle_attacker_pool.emplace_back(circle);
    }
    else if (circle->GetCollisionType() & COLLISION_TYPE::COLLISION_TYPE_DEFENDER)
    {
        this->circle_deffender_pool.emplace_back(circle);
    }
}

void CircleCollider::CheckCollision()
{
    // 実行可能なattackerの取得
    std::vector<std::shared_ptr<CircleComponent>> active_attacker_pool;
    for (std::weak_ptr<CircleComponent> attacker_Wptr : this->circle_attacker_pool)
    {
        std::shared_ptr<CircleComponent> attacker = attacker_Wptr.lock();
        if (!attacker)
        {
            // 削除コンテナに追加
            circle_remove_pool.emplace_back(attacker_Wptr);
            continue;
        }
        if (!attacker->GetIsActive()) continue;

        active_attacker_pool.emplace_back(attacker);
    }

    // 実行可能なdeffenderの取得
    std::vector<std::shared_ptr<CircleComponent>> active_deffender_pool;
    for (std::weak_ptr<CircleComponent> deffender_Wptr : this->circle_deffender_pool)
    {
        std::shared_ptr<CircleComponent> deffender = deffender_Wptr.lock();
        if (!deffender)
        {
            // 削除コンテナに追加
            circle_remove_pool.emplace_back(deffender_Wptr);
            continue;
        }
        if (!deffender->GetIsActive()) continue;

        active_deffender_pool.emplace_back(deffender);
    }
}

void CircleCollider::RemoveDeletedCircle()
{
}
