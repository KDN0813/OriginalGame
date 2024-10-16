#pragma once
#include <memory>
#include <vector>
#include <set>
#include "Collision/CollisionParam.h"

class CircleComponent;

// 円の当たり判定
class CircleCollider
{
public:
    CircleCollider();
    ~CircleCollider() {};

    // 外部で呼ぶ更新関数
    void Update();

    // 当たり判定を行うcircleを追加する関数
    void AddCircle(std::shared_ptr<CircleComponent>& circle);
private:
    // 当たり判定を行う関数
    void CheckCollision();

    // 既に削除されたcircleのコンテナから取り除く関数
    void RemoveDeletedCircle();
private:
    // 同じOBJECT_TYPE (例: PLAYERやENEMY) に属する
    // ぶつける側 (ATTACKER) とぶつけられる側 (DEFENDER) のデータを保持する構造体
    struct CircleCollision
    {
        std::vector<std::weak_ptr<CircleComponent>> circle_attacker_pool;   // ぶつける側 (ATTACKER)
        std::vector<std::weak_ptr<CircleComponent>> circle_defender_pool;   // ぶつけられる側 (DEFENDER)
    };
private:
    std::vector<CircleCollision> circle_collision_pool;
};

