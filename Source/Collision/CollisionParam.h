#pragma once
#include <DirectXMath.h>
#include <memory>

class Object;

using CollisionType = size_t;

enum class COLLISION_OBJECT_TYPE : CollisionType
{
    PLAYER_ATTACK = 0,
    PLAYER_DEFENSE,
    ENEMY_ATTACK,     
    ENEMY_DEFENSE,
    MAX,
    NONE,
};

enum class COLLISION_ROLE
{
    ATTACKER = 0,  // �Փ˂������N���������i�U���ҁj
    RECEIVER       // �Փ˂��󂯂����i��U���ҁj
};

struct CircleParam
{
    DirectX::XMFLOAT2 center;
    float radius;
};

struct CircleHitResult
{
    DirectX::XMFLOAT2 out_position;
    COLLISION_ROLE collision_role;
};

/**
* ���C�̃q�b�g����
*
* .\param position ���C�Ƃƃ|���S���̌�_
* .\param normal �Փ˂����|���S���̖@���x�N�g��
* .\param rotation ��]��
* .\param distance ���C�̎n�_�����_�܂ł̋���
* .\param material_index �Փ˂����|���S���̃}�e���A���ԍ�
*/
struct RayHitResult
{
    DirectX::XMFLOAT3   position = {};
    DirectX::XMFLOAT3   normal = {};
    DirectX::XMFLOAT3   rotation = {};
    float               distance = 0.0f;
    int                 material_index = -1;
};
