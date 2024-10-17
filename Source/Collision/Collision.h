#pragma once
#include "System/MyMath/MYVECTOR3.h"
#include "Component/ModelComponent.h"

#include "Collision/CollisionParam.h"

class Collision
{
public:
    // �~�Ɖ~�̓����蔻��
    static bool IntersectCircleVsCircle(
        CircleParam circleA
        , CircleParam circleB
        , CircleHitResult& hit_resultA
        , CircleHitResult& hit_resultB
    );

    // ���Ƌ��̌�������
    static bool IntersectSphareVsSphere(
        DirectX::XMFLOAT3 positionA
        , float radiusA
        , DirectX::XMFLOAT3 positionB
        , float radiusB
        , DirectX::XMFLOAT3 out_positionB
    );

    // �~���Ɖ~���̌�������
    static bool IntersectCylinderVsCylinder(
        DirectX::XMFLOAT3 positionA
        , float radiusA
        , float heightA
        , DirectX::XMFLOAT3 positionB
        , float radiusB
        , float heightB
        , DirectX::XMFLOAT3 out_positionB
    );

    // ���Ɖ~���̌�������
    static bool IntersectSphereVsCylinder(
        DirectX::XMFLOAT3 sphere_position
        , float sphere_radius
        , DirectX::XMFLOAT3 cylinder_position
        , float cylinder_radius
        , float cylinder_height
        , DirectX::XMFLOAT3 out_cylinder_position
    );

    /**
     * ���C�ƃ��f���̌�������
     * 
     * \param World_start ���C�̎n�_
     * \param World_end ���C�̏I�_
     * \param model ���f���̃|�C���^
     * \param result �q�b�g���U���g(���ʂ��i�[����)
     * \return ��������(����������true)
     */
    static bool IntersectRayVsModel(
        MYVECTOR3 World_start,
        MYVECTOR3 World_end,
        const ModelComponent* model,
        RayHitResult& result
    );
};

