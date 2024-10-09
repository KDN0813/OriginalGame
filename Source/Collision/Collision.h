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
    MYVECTOR3   position        = {};
    MYVECTOR3   normal          = {};
    MYVECTOR3   rotation          = {};
    float               distance        = 0.0f;
    int                 material_index   = -1;
};

class Collision
{
public:
    // 球と球の交差判定
    static bool IntersectSphareVsSphere(
        MYVECTOR3 positionA
        , float radiusA
        , MYVECTOR3 positionB
        , float radiusB
        , MYVECTOR3 out_positionB
    );

    // 円柱と円柱の交差判定
    static bool IntersectCylinderVsCylinder(
        MYVECTOR3 positionA
        , float radiusA
        , float heightA
        , MYVECTOR3 positionB
        , float radiusB
        , float heightB
        , MYVECTOR3 out_positionB
    );

    // 球と円柱の交差判定
    static bool IntersectSphereVsCylinder(
        MYVECTOR3 sphere_position
        , float sphere_radius
        , MYVECTOR3 cylinder_position
        , float cylinder_radius
        , float cylinder_height
        , MYVECTOR3 out_cylinder_position
    );

    /**
     * レイとモデルの交差判定
     * 
     * \param start レイの始点
     * \param end レイの終点
     * \param model モデルのポインタ
     * \param result ヒットリザルト(結果を格納する)
     * \return 交差判定(交差したらtrue)
     */
    static bool IntersectRayVsModel(
        MYVECTOR3 start,
        MYVECTOR3 end,
        const ModelComponent* model,
        HitResult& result
    );
};

