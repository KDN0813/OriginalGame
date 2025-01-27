#include "FinalUpdateEnemyComponent.h"

#include "System\MyMath\MYVECTOR3.h"
#include "Object\GameObject.h"
#include "Object\Constant\StageConstant.h"
#include "GridObjectManager\GridObjectManager.h"

#include "Component\EnemyComponent.h"
#include "Component\TransformComponent.h"
#include "Component\PlayerComponent.h"

void FinalUpdateEnemyComponent::Start()
{
}

void FinalUpdateEnemyComponent::Update(float elapsed_time)
{
    const auto& onwer = GetOwner();
    if (!onwer) return;
    const auto& transform = onwer->GetComponent<Transform3DComponent>(this->transform_Wptr);
    if (!transform) return;

    // �v���C���[�擾
    GameObject::Instance game_object = GameObject::GetInstance();
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
    // 2�_�Ԃ̃O���b�h�̋��������߂�
    const int range = grid_object_maanager->GetDistanceInGrid(pos, player_pos);

    // 2�}�X�ȓ��Ƀv���C���[�����݂����
    if (range <= 3)
    {
        // �Փ˔�����s��
        MYVECTOR3 Pos = pos;
        MYVECTOR3 Player_pos = player_pos;
        MYVECTOR3 Vec = Pos - Player_pos;

        const auto& player_component = player->GetComponent<PlayerComponent>(this->player_Wptr);
        if (!player_component) return;
        const float player_radius = player_component->GetPushRadius();

        const float vec_range = Vec.LengthSq();
        if (vec_range < player_radius * player_radius)
        {
            // �Փ˂��Ă�����A�����o���������s��
            Vec.NormalizeSelf();
            MYVECTOR3 New_pos = Player_pos + Vec * player_radius;
            DirectX::XMFLOAT3 new_pos{};
            New_pos.GetFlaot3(new_pos);
            new_pos.y = StageConstant::STAGE_FLOOR_Y;   // Y���W�͏��̈ʒu�ŌŒ肷��

            transform->SetLocalPosition(new_pos);
        }
    }
}
