#pragma once
#include <DirectXMath.h>
#include <memory>

class Object;

using CollisionType = size_t;
using TargetType = size_t;

// コライダーの種類を定義する列挙型
// オブジェクトが衝突の際にどの役割を果たすかを指定する
enum class COLLISION_TYPE : CollisionType
{
    ATTACKER = 0,    // ぶつける側（アクティブに衝突を引き起こすオブジェクト）
    DEFENDER,        // ぶつけられる側（衝突されるオブジェクト）
    MAX,
    NONE,
};

// 対象となるターゲットの種類を定義する列挙型
// ATTACKER（攻撃者）および DEFENDER（防御者）で使用し、
// それぞれどのタイプの相手をターゲットとするかを指定する
enum class TARGET_TYPE : TargetType
{
    PLAYER = 0,         // プレイヤーをターゲットとする
    ENEMY,              // 敵をターゲットとする
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
    std::weak_ptr<Object> hit_object_Wptr;
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
