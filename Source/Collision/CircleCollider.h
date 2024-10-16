#pragma once
#include <memory>
#include <vector>

class CircleComponent;

// 円の当たり判定
class CircleCollider
{
public:
    CircleCollider() {};
    ~CircleCollider() {};

    // 外部で呼ぶ更新関数
    void Update();

    // 当たり判定を行うcircleを追加する関数
    void AddCircle(std::shared_ptr<CircleComponent> circle);
private:
    // 当たり判定を行う関数
    void CheckCollision();

    // 既に削除されたcircleのコンテナから取り除く関数
    void RemoveDeletedCircle();
private:
    std::vector<std::weak_ptr<CircleComponent>> circle_attacker_pool;   // ぶつける側
    std::vector<std::weak_ptr<CircleComponent>> circle_deffender_pool;  // ぶつけられる側
};

