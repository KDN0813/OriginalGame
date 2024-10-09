#pragma once
#include "System/MyMath/MYVECTOR3.h"
#include "Component/ModelComponent.h"

 /**
 * �q�b�g����
 * 
 * .\param position ���C�Ƃƃ|���S���̌�_
 * .\param normal �Փ˂����|���S���̖@���x�N�g��
 * .\param rotation ��]��
 * .\param distance ���C�̎n�_�����_�܂ł̋���
 * .\param material_index �Փ˂����|���S���̃}�e���A���ԍ�
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
    // ���Ƌ��̌�������
    static bool IntersectSphareVsSphere(
        MYVECTOR3 positionA
        , float radiusA
        , MYVECTOR3 positionB
        , float radiusB
        , MYVECTOR3 out_positionB
    );

    // �~���Ɖ~���̌�������
    static bool IntersectCylinderVsCylinder(
        MYVECTOR3 positionA
        , float radiusA
        , float heightA
        , MYVECTOR3 positionB
        , float radiusB
        , float heightB
        , MYVECTOR3 out_positionB
    );

    // ���Ɖ~���̌�������
    static bool IntersectSphereVsCylinder(
        MYVECTOR3 sphere_position
        , float sphere_radius
        , MYVECTOR3 cylinder_position
        , float cylinder_radius
        , float cylinder_height
        , MYVECTOR3 out_cylinder_position
    );

    /**
     * ���C�ƃ��f���̌�������
     * 
     * \param start ���C�̎n�_
     * \param end ���C�̏I�_
     * \param model ���f���̃|�C���^
     * \param result �q�b�g���U���g(���ʂ��i�[����)
     * \return ��������(����������true)
     */
    static bool IntersectRayVsModel(
        MYVECTOR3 start,
        MYVECTOR3 end,
        const ModelComponent* model,
        HitResult& result
    );
};

