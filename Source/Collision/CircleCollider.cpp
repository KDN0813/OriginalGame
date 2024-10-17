#include "CircleCollider.h"
#include "Collision/Collision.h"

#include "Object/Object.h"
#include "Component/CircleComponent.h"

CircleCollider::CircleCollider()
{
    this->circle_collision_pool.resize(static_cast<size_t>(OBJECT_TYPE::MAX));
}

void CircleCollider::Update()
{
    CheckCollision();
}

void CircleCollider::AddCircle(std::shared_ptr<CircleComponent>& circle)
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
    // ���s�\��attacker�̎擾
    std::vector<std::vector<std::shared_ptr<CircleComponent>>> active_attacker_pool_pool;
    std::vector<bool> is_active_defender_object; // ���Ă��鑤�̃I�u�W�F�N�g���K�p�ł��邩

    const size_t CircleCollisionMax = this->circle_collision_pool.size();
    active_attacker_pool_pool.resize(CircleCollisionMax);
    is_active_defender_object.resize(CircleCollisionMax);

    // �U�����̒����画����s�����̂��R���e�i�ɏW�߂�
    for (size_t i = 0; i < CircleCollisionMax; ++i)
    {
        for (std::weak_ptr<CircleComponent> attacker_Wptr : this->circle_collision_pool[i].circle_attacker_pool)
        {
            // �L���ł��邩����
            std::shared_ptr<CircleComponent> attacker = attacker_Wptr.lock();
            if (!attacker)
            {
                // �폜�R���e�i�ɒǉ�
                continue;
            }
            if (!attacker->GetIsActive()) continue;

            active_attacker_pool_pool[i].emplace_back(attacker);

            // �^�[�Q�b�g�̃I�u�W�F�N�g�^�C�v�̎󂯑��̎g�pflag�𗧂Ă�
            is_active_defender_object[static_cast<size_t>(attacker->GetTargetType())] = true;
        }
    }
    
    // �L���Ȏ󂯑��̃f�[�^�擾
    std::vector<std::vector<std::shared_ptr<CircleComponent>>> active_deffender_pool_pool;

    active_deffender_pool_pool.resize(CircleCollisionMax);

    for (size_t i = 0; i < is_active_defender_object.size(); ++i)
    {
        // ���݂̃^�C�v�̕K�p�ł���t���O�������Ă��Ȃ��ꍇ��΂�
        if (!is_active_defender_object[i]) continue;

        for (std::weak_ptr<CircleComponent> deffender_Wptr : this->circle_collision_pool[i].circle_defender_pool)
        {
            std::shared_ptr<CircleComponent> deffender = deffender_Wptr.lock();
            if (!deffender)
            {
                // �폜�R���e�i�ɒǉ�
                continue;
            }
            if (!deffender->GetIsActive()) continue;

            active_deffender_pool_pool.emplace_back(deffender);
        }
    }

    // �e��ނ̓��Ă鑤�̃R���e�i�𔻒�
    for (std::vector<std::shared_ptr<CircleComponent>> active_attacker_pool : active_attacker_pool_pool)
    {
        // ���Ă鑤��CircleComponent���擾
        for (std::shared_ptr<CircleComponent> attacker : active_attacker_pool)
        {

            // �e��ނ̎󂯂鑤�̃R���e�i�𔻒�
            for (std::vector<std::shared_ptr<CircleComponent>> active_deffender_pool : active_deffender_pool_pool)
            {
                // �󂯂鑤��CircleComponent���擾
                for (std::shared_ptr<CircleComponent>& deffender : active_deffender_pool)
                {
                    // ������s��
                    CircleHitResult hit;
                    if (Collision::IntersectCircleVsCircle(attacker->GetCircleParam(), deffender->GetCircleParam(), hit))
                    {
                        attacker->SetHitFlag(true);
                        deffender->SetHitFlag(true);

                        hit.hit_object_Wptr = deffender->GetOwner();
                        attacker->SetHitResult(hit);
                    }
                }
            }
        }
    }
}

void CircleCollider::RemoveDeletedCircle()
{
}
