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
    ATTACKER = 0,  // 衝突を引き起こした側（攻撃者）
    RECEIVER       // 衝突を受けた側（被攻撃者）
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
* レイのヒット結果
*
* .\param position レイととポリゴンの交点
* .\param normal 衝突したポリゴンの法線ベクトル
* .\param rotation 回転量
* .\param distance レイの始点から交点までの距離
* .\param material_index 衝突したポリゴンのマテリアル番号
*/
struct RayHitResult
{
    DirectX::XMFLOAT3   position = {};
    DirectX::XMFLOAT3   normal = {};
    DirectX::XMFLOAT3   rotation = {};
    float               distance = 0.0f;
    int                 material_index = -1;
};
