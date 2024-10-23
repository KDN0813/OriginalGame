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
    // �U�����Ɩh�䑤��CircleCollisionComponent��L���Ȃ��̂����W�߂ďՓ˔�����s��
    
    // ���s�\��attacker�̎擾
    std::vector<std::vector<std::shared_ptr<CircleCollisionComponent>>> active_attacker_pool_pool; // �L���ȍU�����R���|�[�l���g���i�[����R���e�i
    std::vector<bool> is_active_defender_object;  // �h�䑤�I�u�W�F�N�g���L�������t���O�Ǘ�����z��

    const size_t CircleCollisionMax = this->circle_collision_pool.size(); // CircleCollision�̎�ނ̍ő吔
    active_attacker_pool_pool.resize(CircleCollisionMax);  // �e��ނ̃A�N�e�B�u�ȍU�����R���|�[�l���g�p�R���e�i��������
    is_active_defender_object.resize(CircleCollisionMax);  // �e��ނ̖h�䑤�I�u�W�F�N�g�̃A�N�e�B�u�t���O��������

    // �U�����̒�����L���ȃR���|�[�l���g���R���e�i�ɏW�߂�
    for (size_t i = 0; i < CircleCollisionMax; ++i)
    {
        for (std::weak_ptr<CircleCollisionComponent> attacker_Wptr : this->circle_collision_pool[i].circle_attacker_pool)
        {
            // ��|�C���^�����b�N���ėL�����ǂ�������
            std::shared_ptr<CircleCollisionComponent> attacker = attacker_Wptr.lock();
            if (!attacker)
            {
                // �����ȃ|�C���^�i�폜�ς݂Ȃǁj�̓X�L�b�v
                continue;
            }
            if (!attacker->GetIsActive()) continue;  // ��A�N�e�B�u�ȃR���|�[�l���g�̓X�L�b�v

            attacker->SetHitFlag(false);    // �q�b�g�t���O����������

            // �L���ȍU�����R���|�[�l���g��ǉ�
            active_attacker_pool_pool[i].emplace_back(attacker);

            // �^�[�Q�b�g�̃I�u�W�F�N�g�^�C�v�ɑΉ�����h�䑤���L���ł���t���O�𗧂Ă�
            is_active_defender_object[static_cast<size_t>(attacker->GetTargetType())] = true;
        }
    }

    // �L���Ȗh�䑤�R���|�[�l���g���W�߂邽�߂̃R���e�i
    std::vector<std::vector<std::shared_ptr<CircleCollisionComponent>>> active_deffender_pool_pool;
    active_deffender_pool_pool.resize(CircleCollisionMax);  // �h�䑤�̃f�[�^�R���e�i��������

    // �L���Ȗh�䑤�f�[�^���擾
    for (size_t i = 0; i < is_active_defender_object.size(); ++i)
    {
        // �K�v�Ȗh�䑤���L���łȂ��ꍇ�̓X�L�b�v
        if (!is_active_defender_object[i]) continue;

        // �L���Ȗh�䑤�R���|�[�l���g���W�߂�
        for (std::weak_ptr<CircleCollisionComponent> deffender_Wptr : this->circle_collision_pool[i].circle_defender_pool)
        {
            std::shared_ptr<CircleCollisionComponent> deffender = deffender_Wptr.lock();
            if (!deffender)
            {
                // �����ȃ|�C���^�i�폜�ς݂Ȃǁj�̓X�L�b�v
                continue;
            }
            if (!deffender->GetIsActive()) continue;  // ��A�N�e�B�u�ȃR���|�[�l���g�̓X�L�b�v
            
            deffender->SetHitFlag(false);    // �q�b�g�t���O����������

            // �L���Ȗh�䑤�R���|�[�l���g��ǉ�
            active_deffender_pool_pool[i].emplace_back(deffender);
        }
    }

    // �e��ނ̍U�����R���|�[�l���g�ɑ΂��锻��
    for (std::vector<std::shared_ptr<CircleCollisionComponent>> active_attacker_pool : active_attacker_pool_pool)
    {
        // �U������CircleCollisionComponent���擾
        for (std::shared_ptr<CircleCollisionComponent> attacker : active_attacker_pool)
        {
            // �e��ނ̖h�䑤�R���|�[�l���g�ɑ΂��锻��
            for (std::vector<std::shared_ptr<CircleCollisionComponent>> active_deffender_pool : active_deffender_pool_pool)
            {
                // �h�䑤��CircleCollisionComponent���擾
                for (std::shared_ptr<CircleCollisionComponent>& deffender : active_deffender_pool)
                {
                    // �U�����Ɩh�䑤�̏Փ˔�����s��
                    CircleHitResult attacker_hit;
                    CircleHitResult deffender_hit;
                    if (Collision::IntersectCircleVsCircle(attacker->GetCircleParam(), deffender->GetCircleParam(), attacker_hit, deffender_hit))
                    {
                        // �Փ˂����ꍇ�A�U�����Ɩh�䑤�Ƀq�b�g�t���O��ݒ�
                        attacker->SetHitFlag(true);
                        deffender->SetHitFlag(true);

                        // �U�����̃��U���g�ݒ�
                        attacker_hit.hit_object_Wptr = deffender->GetOwner();
                        attacker->SetHitResult(attacker_hit);

                        // �h�䑤�̃��U���g�ݒ�
                        deffender_hit.hit_object_Wptr = attacker->GetOwner();
                        deffender->SetHitResult(deffender_hit);
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
                    label = object_name.c_str();
                    label += "##CollisonObject";
                    if (ImGui::CollapsingHeader(label.c_str()))
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
                        std::string label = "Hit Frag##" + object_name;
                        ImGui::Checkbox(label.c_str(), &hit_frag);
                    }
                    ImGui::Unindent(30.0f);
                }

            }

            ImGui::Unindent(30.0f);
        }
    }
}

#endif // DEBUG