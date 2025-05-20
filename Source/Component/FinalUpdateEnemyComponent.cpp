#include "FinalUpdateEnemyComponent.h"

#include <cmath>
#include "System\MyMath\MYVECTOR3.h"
#include "Object\GameObjectRegistry.h"
#include "Object\Constant\StageConstant.h"
#include "GridObjectManager\GridObjectManager.h"

#include "Component\EnemyComponent.h"
#include "Component\TransformComponent.h"
#include "Component\CharacterComponent.h"

void FinalUpdateEnemyComponent::Start()
{
    // ���g�̔��a�擾
    const auto& owner = GetOwner();
    if (!owner) return;
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    const float radius = character->GetRadius();

    // �v���C���[�̔��a�擾
    GameObjectRegistry::Instance game_object = GameObjectRegistry::GetInstance();
    const auto& player = game_object->GetPlayer();
    if (!player) return;
    const auto& player_character = player->GetComponent<CharacterComponent>(this->player_character_Wptr);
    if (!player_character) return;
    const float player_radius = player_character->GetRadius();

    // �v���C���[�Ƃ̓����蔻����s���͈͂��v�Z
    this->collision_range = static_cast<int>(std::ceil(static_cast<double>(player_radius + radius) / GridObjectManager::CELL_SIZE));
}

void FinalUpdateEnemyComponent::Update(float elapsed_time)
{
    const auto& owner = GetOwner();
    if (!owner) return;
    const auto& transform = owner->GetComponent<Transform3DComponent>(this->transform_Wptr);
    if (!transform) return;
    // ���g�̔��a�擾
    const auto& character = owner->GetComponent<CharacterComponent>(this->character_Wptr);
    if (!character) return;
    const float radius = character->GetRadius();

    // �v���C���[�擾
    GameObjectRegistry::Instance game_object = GameObjectRegistry::GetInstance();
    const auto& player = game_object->GetPlayer();
    if (!player) return;
    // �v���C���[�̃g�����X�t�H�[���擾
    const auto& player_transform = player->GetComponent<Transform3DComponent>(this->player_transform_Wptr);
    if (!player_transform) return;

    // ���g�̃Z������
    DirectX::XMFLOAT3 pos = transform->GetWorldPosition();
    GridObjectManager::Instance grid_object_maanager = GridObjectManager::GetInstance();
    // �v���C���[�̃Z������
    DirectX::XMFLOAT3 player_pos = player_transform->GetWorldPosition();
    // �O���b�h�Ԃ̋���
    const int grid_spacing = grid_object_maanager->GetDistanceInGrid(pos, player_pos);

    // �v���C���[�̈��͈͓��Ɏ��g�����݂����
    if (grid_spacing <= this->collision_range)
    {
        // �Փ˔�����s��
        MYVECTOR3 Pos = pos;
        MYVECTOR3 Player_pos = player_pos;
        MYVECTOR3 Vec = Pos - Player_pos;

        const auto& player_character = player->GetComponent<CharacterComponent>(this->player_character_Wptr);
        if (!player_character) return;
        const float player_radius = player_character->GetRadius();

        const float  push_radius = radius + player_radius;

        const float vec_range = Vec.LengthSq();
        if (vec_range < push_radius * push_radius)
        {
            // �Փ˂��Ă�����A�����o���������s��
            Vec.NormalizeSelf();
            MYVECTOR3 New_pos = Player_pos + Vec * push_radius;
            DirectX::XMFLOAT3 new_pos{};
            New_pos.GetFlaot3(new_pos);
            new_pos.y = StageConstant::STAGE_FLOOR_Y;   // Y���W�͏��̈ʒu�ŌŒ肷��

            transform->SetLocalPosition(new_pos);
        }

#ifdef _DEBUG
        character->SetIsDebugPrimitive(true);   // ��苗�����ɂ��鎞�������a��`�悷��
#endif // _DEBUG
    }
#ifdef _DEBUG
    else
    {
        character->SetIsDebugPrimitive(false);
    }

#endif // _DEBUG
}
