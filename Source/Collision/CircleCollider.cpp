#include "CircleCollider.h"
#include "Collision/Collision.h"

#include "Object/Object.h"
#include "Component/CircleCollisionComponent.h"

CircleCollider::CircleCollider()
{
    this->circle_collision_pool.resize(static_cast<size_t>(OBJECT_TYPE::MAX));
}

void CircleCollider::Update()
{
    CheckCollision();
}

void CircleCollider::AddCircle(std::shared_ptr<CircleCollisionComponent>& circle)
{
    if (!circle) return;
    if (circle->GetCollisionType() <= COLLISION_TYPE::MAX) return;

    CircleCollision& circle_collision = this->circle_collision_pool[static_cast<size_t>(circle->GetSelfType())];

    if (circle->GetCollisionType() == COLLISION_TYPE::ATTACKER)
    {
        circle_collision.circle_attacker_pool.emplace_back(circle);
    }
    else if (circle->GetCollisionType() == COLLISION_TYPE::DEFENDER)
    {
        circle_collision.circle_defender_pool.emplace_back(circle);
    }
}

void CircleCollider::CheckCollision()
{
    //// 実行可能なattackerの取得
    //std::vector<std::vector<std::shared_ptr<CircleCollisionComponent>>> active_attacker_pool_pool;
    //std::vector<bool> is_active_defender_object; // あてられる側のオブジェクトが必用であるか

    //const size_t CircleCollisionMax = this->circle_collision_pool.size();
    //active_attacker_pool_pool.resize(CircleCollisionMax);
    //is_active_defender_object.resize(CircleCollisionMax);

    //// 攻撃側の中から判定を行うものをコンテナに集める
    //for (size_t i = 0; i < CircleCollisionMax; ++i)
    //{
    //    for (std::weak_ptr<CircleCollisionComponent> attacker_Wptr : this->circle_collision_pool[i].circle_attacker_pool)
    //    {
    //        // 有効であるか判定
    //        std::shared_ptr<CircleCollisionComponent> attacker = attacker_Wptr.lock();
    //        if (!attacker)
    //        {
    //            // 削除コンテナに追加
    //            continue;
    //        }
    //        if (!attacker->GetIsActive()) continue;

    //        active_attacker_pool_pool[i].emplace_back(attacker);

    //        // ターゲットのオブジェクトタイプの受け側の使用flagを立てる
    //        is_active_defender_object[static_cast<size_t>(attacker->GetTargetType())] = true;
    //    }
    //}
    //
    //// 有効な受け側のデータ取得
    //std::vector<std::vector<std::shared_ptr<CircleCollisionComponent>>> active_deffender_pool_pool;

    //active_deffender_pool_pool.resize(CircleCollisionMax);

    //for (size_t i = 0; i < is_active_defender_object.size(); ++i)
    //{
    //    // 現在のタイプの必用であるフラグが立っていない場合飛ばす
    //    if (!is_active_defender_object[i]) continue;

    //    for (std::weak_ptr<CircleCollisionComponent> deffender_Wptr : this->circle_collision_pool[i].circle_defender_pool)
    //    {
    //        std::shared_ptr<CircleCollisionComponent> deffender = deffender_Wptr.lock();
    //        if (!deffender)
    //        {
    //            // 削除コンテナに追加
    //            continue;
    //        }
    //        if (!deffender->GetIsActive()) continue;

    //        active_deffender_pool_pool.emplace_back(deffender);
    //    }
    //}

    //// 各種類の当てる側のコンテナを判定
    //for (std::vector<std::shared_ptr<CircleCollisionComponent>> active_attacker_pool : active_attacker_pool_pool)
    //{
    //    // 当てる側のCircleCollisionComponentを取得
    //    for (std::shared_ptr<CircleCollisionComponent> attacker : active_attacker_pool)
    //    {

    //        // 各種類の受ける側のコンテナを判定
    //        for (std::vector<std::shared_ptr<CircleCollisionComponent>> active_deffender_pool : active_deffender_pool_pool)
    //        {
    //            // 受ける側のCircleCollisionComponentを取得
    //            for (std::shared_ptr<CircleCollisionComponent>& deffender : active_deffender_pool)
    //            {
    //                // 判定を行う
    //                CircleHitResult hit;
    //                if (Collision::IntersectCircleVsCircle(attacker->GetCircleParam(), deffender->GetCircleParam(), hit))
    //                {
    //                    attacker->SetHitFlag(true);
    //                    deffender->SetHitFlag(true);

    //                    hit.hit_object_Wptr = deffender->GetOwner();
    //                    attacker->SetHitResult(hit);
    //                }
    //            }
    //        }
    //    }
    //}
}

void CircleCollider::RemoveDeletedCircle()
{
}
