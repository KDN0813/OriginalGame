#include "CircleCollider.h"
#include "Collision/Collision.h"

#include "Object/Object.h"
#include "Component/CircleCollisionComponent.h"
#ifdef _DEBUG
#include <imgui.h>
#include <magic_enum.hpp>
#endif // _DEBUG


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
    if (circle->GetCollisionType() >= COLLISION_TYPE::MAX) return;

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
    // TODO(10/17)
    // �R�[�h����

    //// ���s�\��attacker�̎擾
    std::vector<std::vector<std::shared_ptr<CircleCollisionComponent>>> active_attacker_pool_pool;
    std::vector<bool> is_active_defender_object; // ���Ă��鑤�̃I�u�W�F�N�g���K�p�ł��邩

    const size_t CircleCollisionMax = this->circle_collision_pool.size();
    active_attacker_pool_pool.resize(CircleCollisionMax);
    is_active_defender_object.resize(CircleCollisionMax);

    // �U�����̒����画����s�����̂��R���e�i�ɏW�߂�
    for (size_t i = 0; i < CircleCollisionMax; ++i)
    {
        for (std::weak_ptr<CircleCollisionComponent> attacker_Wptr : this->circle_collision_pool[i].circle_attacker_pool)
        {
            // �L���ł��邩����
            std::shared_ptr<CircleCollisionComponent> attacker = attacker_Wptr.lock();
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
    std::vector<std::vector<std::shared_ptr<CircleCollisionComponent>>> active_deffender_pool_pool;

    active_deffender_pool_pool.resize(CircleCollisionMax);

    for (size_t i = 0; i < is_active_defender_object.size(); ++i)
    {
        // ���݂̃^�C�v�̕K�p�ł���t���O�������Ă��Ȃ��ꍇ��΂�
        if (!is_active_defender_object[i]) continue;

        for (std::weak_ptr<CircleCollisionComponent> deffender_Wptr : this->circle_collision_pool[i].circle_defender_pool)
        {
            std::shared_ptr<CircleCollisionComponent> deffender = deffender_Wptr.lock();
            if (!deffender)
            {
                // �폜�R���e�i�ɒǉ�
                continue;
            }
            if (!deffender->GetIsActive()) continue;

            active_deffender_pool_pool[i].emplace_back(deffender);
        }
    }

    // �e��ނ̓��Ă鑤�̃R���e�i�𔻒�
    for (std::vector<std::shared_ptr<CircleCollisionComponent>> active_attacker_pool : active_attacker_pool_pool)
    {
        // ���Ă鑤��CircleCollisionComponent���擾
        for (std::shared_ptr<CircleCollisionComponent> attacker : active_attacker_pool)
        {

            // �e��ނ̎󂯂鑤�̃R���e�i�𔻒�
            for (std::vector<std::shared_ptr<CircleCollisionComponent>> active_deffender_pool : active_deffender_pool_pool)
            {
                // �󂯂鑤��CircleCollisionComponent���擾
                for (std::shared_ptr<CircleCollisionComponent>& deffender : active_deffender_pool)
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

#ifdef _DEBUG

void CircleCollider::DrawDebugGUI()
{
    for (size_t circle_collision_index = 0;circle_collision_index< circle_collision_pool.size();++circle_collision_index)
    {
        std::string header_name;
        header_name += magic_enum::enum_name(static_cast<OBJECT_TYPE>(circle_collision_index));
        if (ImGui::CollapsingHeader(header_name.c_str()))
        {
            ImGui::Indent(30.0f);

            auto circle_collision = circle_collision_pool[circle_collision_index];
            std::string label = "ATTACKER##" + std::to_string(circle_collision_index);
            if (ImGui::CollapsingHeader(label.c_str()))
            {
                auto circle_attacker_pool = circle_collision.circle_attacker_pool;

                for (auto circle_attacker_Wptr : circle_attacker_pool)
                {
                    auto circle_attacker = circle_attacker_Wptr.lock();
                    if (!circle_attacker) continue;

                    std::string object_name = "null";
                    auto object = circle_attacker->GetOwner();
                    if (object)
                    {
                        object_name = object->GetName();
                    }

                    ImGui::Indent(30.0f);
                    if (ImGui::CollapsingHeader(object_name.c_str()))
                    {
                        bool hit_frag = circle_attacker->GetHitFlag();
                        std::string label = "Hit Frag##" + object_name;
                        ImGui::Checkbox(label.c_str(), &hit_frag);
                    }   
                    ImGui::Unindent(30.0f);
                }
            }
            label = "DEFENDER##" + std::to_string(circle_collision_index);
            if (ImGui::CollapsingHeader(label.c_str()))
            {
                auto circle_defender_pool = circle_collision.circle_defender_pool;

                for (auto circle_defender_Wptr : circle_defender_pool)
                {
                    auto circle_defender = circle_defender_Wptr.lock();
                    if (!circle_defender) continue;

                    std::string object_name = "null";
                    auto object = circle_defender->GetOwner();
                    if (object)
                    {
                        object_name = object->GetName();
                    }

                    ImGui::Indent(30.0f);
                    if (ImGui::CollapsingHeader(object_name.c_str()))
                    {
                        bool hit_frag = circle_defender->GetHitFlag();
                    }
                    ImGui::Unindent(30.0f);
                }

            }

            ImGui::Unindent(30.0f);
        }
    }
}

#endif // DEBUG