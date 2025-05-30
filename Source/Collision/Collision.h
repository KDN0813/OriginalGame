#pragma once
#include "System/MyMath/MYVECTOR3.h"
#include "Component/ModelComponent.h"

#include "Collision/CollisionParam.h"

class Collision
{
public:
    // ~Æ~Ì½è»è
    static bool IntersectCircleVsCircle(
        CircleParam circleA
        , CircleParam circleB
        , CircleHitResult& hit_resultA
        , CircleHitResult& hit_resultB
    );

    // ÆÌð·»è
    static bool IntersectSphareVsSphere(
        DirectX::XMFLOAT3 positionA
        , float radiusA
        , DirectX::XMFLOAT3 positionB
        , float radiusB
        , DirectX::XMFLOAT3 out_positionB
    );

    // ~Æ~Ìð·»è
    static bool IntersectCylinderVsCylinder(
        DirectX::XMFLOAT3 positionA
        , float radiusA
        , float heightA
        , DirectX::XMFLOAT3 positionB
        , float radiusB
        , float heightB
        , DirectX::XMFLOAT3 out_positionB
    );

    // Æ~Ìð·»è
    static bool IntersectSphereVsCylinder(
        DirectX::XMFLOAT3 sphere_position
        , float sphere_radius
        , DirectX::XMFLOAT3 cylinder_position
        , float cylinder_radius
        , float cylinder_height
        , DirectX::XMFLOAT3 out_cylinder_position
    );

    /**
     * CÆfÌð·»è
     * 
     * \param World_start CÌn_
     * \param World_end CÌI_
     * \param model fÌ|C^
     * \param result qbgUg(Êði[·é)
     * \return ð·»è(ð·µ½çtrue)
     */
    static bool IntersectRayVsModel(
        MYVECTOR3 World_start,
        MYVECTOR3 World_end,
        const ModelComponent* model,
        RayHitResult& result
    );
};

