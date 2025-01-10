#pragma once
#include "CollisionActionBase.h"

// �v���C���[���Փ˂������Ƃ��Ɏ��s�����A�N�V����
class PlayerOnHitAction : public CollisionAction
{
public:
    void Action() override;
};


// �v���C���[���Փ˂��󂯂��Ƃ��Ɏ��s�����A�N�V����
class PlayerHitReactionAction : public CollisionAction
{
public:
    void Action() override;
};


// �G�l�~�[���Փ˂������Ƃ��Ɏ��s�����A�N�V����
class EnemyOnHitAction : public CollisionAction
{
public:
    void Action() override;
};


// �G�l�~�[���Փ˂��󂯂��Ƃ��Ɏ��s�����A�N�V����
class EnemyHitReactionAction : public CollisionAction
{
public:
    void Action() override;
};