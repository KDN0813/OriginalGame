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
    // 攻撃側と防御側のCircleCollisionComponentを有効なものだけ集めて衝突判定を行う
    
    // 実行可能なattackerの取得
    std::vector<std::vector<std::shared_ptr<CircleCollisionComponent>>> active_attacker_pool_pool; // 有効な攻撃側コンポーネントを格納するコンテナ
    std::vector<bool> is_active_defender_object;  // 防御側オブジェクトが有効かをフラグ管理する配列

    const size_t CircleCollisionMax = this->circle_collision_pool.size(); // CircleCollisionの種類の最大数
    active_attacker_pool_pool.resize(CircleCollisionMax);  // 各種類のアクティブな攻撃側コンポーネント用コンテナを初期化
    is_active_defender_object.resize(CircleCollisionMax);  // 各種類の防御側オブジェクトのアクティブフラグを初期化

    // 攻撃側の中から有効なコンポーネントをコンテナに集める
    for (size_t i = 0; i < CircleCollisionMax; ++i)
    {
        for (std::weak_ptr<CircleCollisionComponent> attacker_Wptr : this->circle_collision_pool[i].circle_attacker_pool)
        {
            // 弱ポインタをロックして有効かどうか判定
            std::shared_ptr<CircleCollisionComponent> attacker = attacker_Wptr.lock();
            if (!attacker)
            {
                // 無効なポインタ（削除済みなど）はスキップ
                continue;
            }
            if (!attacker->GetIsActive()) continue;  // 非アクティブなコンポーネントはスキップ

            attacker->SetHitFlag(false);    // ヒットフラグを解除する

            // 有効な攻撃側コンポーネントを追加
            active_attacker_pool_pool[i].emplace_back(attacker);

            // ターゲットのオブジェクトタイプに対応する防御側が有効であるフラグを立てる
            is_active_defender_object[static_cast<size_t>(attacker->GetTargetType())] = true;
        }
    }

    // 有効な防御側コンポーネントを集めるためのコンテナ
    std::vector<std::vector<std::shared_ptr<CircleCollisionComponent>>> active_deffender_pool_pool;
    active_deffender_pool_pool.resize(CircleCollisionMax);  // 防御側のデータコンテナを初期化

    // 有効な防御側データを取得
    for (size_t i = 0; i < is_active_defender_object.size(); ++i)
    {
        // 必要な防御側が有効でない場合はスキップ
        if (!is_active_defender_object[i]) continue;

        // 有効な防御側コンポーネントを集める
        for (std::weak_ptr<CircleCollisionComponent> deffender_Wptr : this->circle_collision_pool[i].circle_defender_pool)
        {
            std::shared_ptr<CircleCollisionComponent> deffender = deffender_Wptr.lock();
            if (!deffender)
            {
                // 無効なポインタ（削除済みなど）はスキップ
                continue;
            }
            if (!deffender->GetIsActive()) continue;  // 非アクティブなコンポーネントはスキップ
            
            deffender->SetHitFlag(false);    // ヒットフラグを解除する

            // 有効な防御側コンポーネントを追加
            active_deffender_pool_pool[i].emplace_back(deffender);
        }
    }

    // 各種類の攻撃側コンポーネントに対する判定
    for (std::vector<std::shared_ptr<CircleCollisionComponent>> active_attacker_pool : active_attacker_pool_pool)
    {
        // 攻撃側のCircleCollisionComponentを取得
        for (std::shared_ptr<CircleCollisionComponent> attacker : active_attacker_pool)
        {
            // 各種類の防御側コンポーネントに対する判定
            for (std::vector<std::shared_ptr<CircleCollisionComponent>> active_deffender_pool : active_deffender_pool_pool)
            {
                // 防御側のCircleCollisionComponentを取得
                for (std::shared_ptr<CircleCollisionComponent>& deffender : active_deffender_pool)
                {
                    // 攻撃側と防御側の衝突判定を行う
                    CircleHitResult attacker_hit;
                    CircleHitResult deffender_hit;
                    if (Collision::IntersectCircleVsCircle(attacker->GetCircleParam(), deffender->GetCircleParam(), attacker_hit, deffender_hit))
                    {
                        // 衝突した場合、攻撃側と防御側にヒットフラグを設定
                        attacker->SetHitFlag(true);
                        deffender->SetHitFlag(true);

                        // 攻撃側のリザルト設定
                        attacker_hit.hit_object_Wptr = deffender->GetOwner();
                        attacker->SetHitResult(attacker_hit);

                        // 防御側のリザルト設定
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