#pragma once
#include <DirectXMath.h>
#include <memory>

class Object;

using CollisionType = size_t;

enum class COLLISION_OBJECT_TYPE : CollisionType
{
    PLAYER = 0,
    ENEMY,     
    MAX,
    NONE,
};

struct CircleParam
{
    DirectX::XMFLOAT2 center;
    float radius;
};

struct CircleHitResult
{
    DirectX::XMFLOAT2 out_position;
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
