#include "GameObject.h"
#ifdef _DEBUG
#include "Debug/ImGuiHelper.h"
#endif // DEBUG
#include "System\MyMath\MyMathf.h"
#include "Object\Constant\EnemyConstant.h"

#include "Component\TransformComponent.h"

GameObject::GameObject()
    :Singleton(this)
{
}

void GameObject::Update()
{
    // �L���łȂ�weak_ptr����������
    this->enemy_Wptr_pool.erase(
        std::remove_if(this->enemy_Wptr_pool.begin(), this->enemy_Wptr_pool.end(),
            [](const std::weak_ptr<Object>& weakPtr)
            {
                return weakPtr.expired();  // ������weak_ptr���ǂ������m�F
            }),
        this->enemy_Wptr_pool.end());
}

void GameObject::AddCreateEnemy(float elapsed_time, ObjectManager& object_manager)
{
    const auto& player = GetPlayer();
    if (!player) return;
    const auto& player_transform = player->GetComponent<Transform3DComponent>(this->player_transform_Wptr);
    if (!player_transform) return;
    const DirectX::XMFLOAT3 PLAYER_POS = player_transform->GetWorldPosition();

    // �G�̐���
    this->create_enemy_cool_time -= elapsed_time;
    if (create_enemy_cool_time <= 0.0f)
    {
        const size_t NOW_ENEMY_COUNT = this->GetEnemyWptPool().size();
        const size_t ENEMY_MAX = EnemyConstant::ENEMY_MAX;
#ifdef _DEBUG
        const size_t CREATE_ENEMY_MAX = MyMath::RandomRange(10, 30);	// 1�x�ɐ����ł���G�l�~�[�̍ő吔
#else
        const size_t CREATE_ENEMY_MAX = MyMath::RandomRange(30, 100);	// 1�x�ɐ����ł���G�l�~�[�̍ő吔
#endif // _DEBUG

        for (int i = 0; i < ENEMY_MAX - NOW_ENEMY_COUNT; ++i)
        {
            if (CREATE_ENEMY_MAX <= i)break;

            const float player_area_rage = 20.0f;
            const DirectX::XMFLOAT3 spawn_point = MyMath::GetNonOverlappingPointInRing(PLAYER_POS, player_area_rage, EnemyConstant::NEAR_PLAYER_TERRITORY_RENGR);

            const auto& enemy = EnemyConstant::CreateEnemy(spawn_point, object_manager.Create());
            SetEnemy(enemy);
        }

        const float COOL_TIME = MyMath::RandomRange(3.0f, 5.0f);
        create_enemy_cool_time = COOL_TIME;
    }
}

void GameObject::EnemyAllClear()
{
    for (auto& enemy_Wptr : this->enemy_Wptr_pool)
    {
        const auto& enemy = enemy_Wptr.lock();
        if (!enemy) continue;
        enemy->SetIsRemove(true);
    }
}

#ifdef _DEBUG

void GameObject::DebugDrawGUI()
{
    if (ImGui::Begin("GameObject"))
    {
        if (ImGui::CollapsingHeader("Player##GameObject"))
        {
            std::string object_name = "Not Object";
            if (std::shared_ptr<Object> player = this->player_Wptr.lock())
            {
                object_name = player->GetName();
            }
            ImGui::InputTextString("Object Name##GameObjectPlayer", object_name);
        }

        if (ImGui::CollapsingHeader("Stage##GameObject"))
        {
            std::string object_name = "Not Object";
            if (std::shared_ptr<Object> stage = this->stage_foor_Wptr.lock())
            {
                object_name = stage->GetName();
            }
            ImGui::InputTextString("Object Name##GameObjectStageFoor", object_name);

            object_name = "Not Object";
            if (std::shared_ptr<Object> stage = this->stage_wall_Wptr.lock())
            {
                object_name = stage->GetName();
            }
            ImGui::InputTextString("Object Name##GameObjectStageWall", object_name);
        }

        if (ImGui::CollapsingHeader("Enemy##GameObject"))
        {
            size_t enemy_count = this->enemy_Wptr_pool.size();
            ImGui::InputSize_t("Enemy Count", enemy_count);

            int enemy_id = 0;   // �G�l�~�[��ID(ImGui�ŕʃf�[�^�Ƃ��Ĉ����p)
            for (const std::weak_ptr<Object>& enemy_Wptr : this->enemy_Wptr_pool)
            {
                if (std::shared_ptr<Object> enemy = enemy_Wptr.lock())
                {
                    std::string object_name = enemy->GetName();
                    std::string label = +"Object Name##GameObjectenemy" + std::to_string(enemy_id);
                    ImGui::InputTextString(label.c_str(), object_name);
                }
                ++enemy_id;
            }
        }
    }
    ImGui::End();
}

#endif // DEBUG