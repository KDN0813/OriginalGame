#pragma once

using CollisionType = unsigned int;
using TargetType = unsigned int;

// コライダーの種類を定義する列挙型
// オブジェクトが衝突の際にどの役割を果たすかを指定する
enum COLLISION_TYPE : CollisionType
{
    COLLISION_TYPE_NONE = 0,                                                        // 当たり判定なし
    COLLISION_TYPE_ATTACKER = 1 << 0,                                               // ぶつける側（アクティブに衝突を引き起こすオブジェクト）
    COLLISION_TYPE_DEFENDER = 1 << 1,                                               // ぶつけられる側（衝突されるオブジェクト）
    COLLISION_TYPE_BOTH = COLLISION_TYPE_ATTACKER | COLLISION_TYPE_DEFENDER,        // 両方に当たり判定がある場合（ぶつける側とぶつけられる側の両方）
    COLLISION_TYPE_DEFAULT = COLLISION_TYPE_BOTH,                                   // デフォルトでは両方に判定を行う
};

// 対象となるターゲットの種類を定義する列挙型
// ATTACKER（攻撃者）および DEFENDER（防御者）で使用し、
// それぞれどのタイプの相手をターゲットとするかを指定する
enum TARGET_TYPE : TargetType
{
    TARGET_NONE = 0,                                // ターゲットなし
    TARGET_PLAYER = 1 << 0,                         // プレイヤーをターゲットとする
    TARGET_ENEMY = 1 << 1,                          // 敵をターゲットとする
    TARGET_ALL = TARGET_PLAYER | TARGET_ENEMY,      // すべてのターゲット
    TARGET_DEFAULT = TARGET_ALL                     // デフォルトではすべてのターゲットを指定
};

class CircleParam
{
    DirectX::XMFLOAT3 center;
    float radius;
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
