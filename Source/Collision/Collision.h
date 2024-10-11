#pragma once
#include "System/MyMath/MYVECTOR3.h"
#include "Component/ModelComponent.h"

 /**
 * ヒット結果
 * 
 * .\param position レイととポリゴンの交点
 * .\param normal 衝突したポリゴンの法線ベクトル
 * .\param rotation 回転量
 * .\param distance レイの始点から交点までの距離
 * .\param material_index 衝突したポリゴンのマテリアル番号
 */
struct HitResult
{
    DirectX::XMFLOAT3   position        = {};
    DirectX::XMFLOAT3   normal          = {};
    DirectX::XMFLOAT3   rotation          = {};
    float               distance        = 0.0f;
    int                 material_index   = -1;
};

class Collision
{
public:
    // 球と球の交差判定
    static bool IntersectSphareVsSphere(
        DirectX::XMFLOAT3 positionA
        , float radiusA
        , DirectX::XMFLOAT3 positionB
        , float radiusB
        , DirectX::XMFLOAT3 out_positionB
    );

    // 円柱と円柱の交差判定
    static bool IntersectCylinderVsCylinder(
        DirectX::XMFLOAT3 positionA
        , float radiusA
        , float heightA
        , DirectX::XMFLOAT3 positionB
        , float radiusB
        , float heightB
        , DirectX::XMFLOAT3 out_positionB
    );

    // 球と円柱の交差判定
    static bool IntersectSphereVsCylinder(
        DirectX::XMFLOAT3 sphere_position
        , float sphere_radius
        , DirectX::XMFLOAT3 cylinder_position
        , float cylinder_radius
        , float cylinder_height
        , DirectX::XMFLOAT3 out_cylinder_position
    );

    /**
     * レイとモデルの交差判定
     * 
     * \param World_start レイの始点
     * \param World_end レイの終点
     * \param model モデルのポインタ
     * \param result ヒットリザルト(結果を格納する)
     * \return 交差判定(交差したらtrue)
     */
    static bool IntersectRayVsModel(
        MYVECTOR3 World_start,
        MYVECTOR3 World_end,
        const ModelComponent* model,
        HitResult& result
    );
};

