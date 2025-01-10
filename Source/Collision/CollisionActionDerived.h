#pragma once
#include "CollisionActionBase.h"

// プレイヤーが衝突させたときに実行されるアクション
class PlayerOnHitAction : public CollisionAction
{
public:
    void Action() override;
};


// プレイヤーが衝突を受けたときに実行されるアクション
class PlayerHitReactionAction : public CollisionAction
{
public:
    void Action() override;
};


// エネミーが衝突させたときに実行されるアクション
class EnemyOnHitAction : public CollisionAction
{
public:
    void Action() override;
};


// エネミーが衝突を受けたときに実行されるアクション
class EnemyHitReactionAction : public CollisionAction
{
public:
    void Action() override;
};