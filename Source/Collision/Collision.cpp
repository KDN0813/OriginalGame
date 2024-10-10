#include "Collision.h"
#include "Model/ModelResource.h"

// ���Ƌ��̌�������
bool Collision::IntersectSphareVsSphere(
    DirectX::XMFLOAT3 positionA
    , float radiusA
    , DirectX::XMFLOAT3 positionB
    , float radiusB
    , DirectX::XMFLOAT3 out_positionB)
{
    // A����B�̒P�ʃx�N�g�����Z�o
    MYVECTOR3 PositionB = positionB;
    MYVECTOR3 PositionA = positionA;
    MYVECTOR3 vec = PositionB - PositionA;

    // ��������[05]
    float range = radiusA + radiusB;
    // LengthSp����悵���l�Ȃ̂�range����悷��
    if (vec.LengthSq() > range * range)
    {
        return false;
    }

    // A��B�������o��
    MYVECTOR3 Out_positionB = PositionA + (vec * range);
    out_positionB = Out_positionB.GetFlaot3();

    return true;
}

// �~���Ɖ~���̌�������
bool Collision::IntersectCylinderVsCylinder(
    DirectX::XMFLOAT3 positionA,
    float radiusA,
    float heightA,
    DirectX::XMFLOAT3 positionB,
    float radiusB,
    float heightB,
    DirectX::XMFLOAT3 out_positionB
)
{
    MYVECTOR3 PositionA = positionA;
    MYVECTOR3 PositionB = positionB;

    // A�̑����Ƃ�B�̓��̏�ɓ������Ă��Ȃ�
    if ((PositionA.GetY()) > (PositionB.GetY() + heightB))
    {
        return false;
    }

    // A�̓���B�̑����Ƃ�艺�Ȃ瓖�����Ă��Ȃ�
    if ((PositionA.GetY() + heightB) < (PositionB.GetY()))
    {
        return false;
    }

    // XZ���ʂł͈̔̓`�F�b�N[06]
    MYVECTOR2 vec = PositionB.GetMyVector2XZ() - PositionA.GetMyVector2XZ();
    float lengthXZ = vec.Length();
    float range = radiusA + radiusB;
    if (lengthXZ > range)
    {
        return false;
    }

    // A��B�������o��
    vec.Normalize();
    vec *= range;
    MYVECTOR3 Out_positionB = MYVECTOR3(PositionA.GetX() + vec.GetX(), PositionB.GetY(), PositionA.GetZ() + vec.GetY());
    out_positionB = Out_positionB.GetFlaot3();
    return true;
}

// ���Ɖ~���̌�������
bool Collision::IntersectSphereVsCylinder(
    DirectX::XMFLOAT3 sphere_position
    , float sphere_radius
    , DirectX::XMFLOAT3 cylinder_position
    , float cylinder_radius
    , float cylinder_height
    , DirectX::XMFLOAT3 out_cylinder_position
)
{
    MYVECTOR3 Sphere_position = sphere_position;
    MYVECTOR3 Cylinder_position = cylinder_position;
    // ���̑����Ƃ��~���̓��̏�ɓ������Ă��Ȃ�
    if ((Sphere_position.GetY() - sphere_radius) > (Cylinder_position.GetY() + cylinder_height))
    {
        return false;
    }

    // ���̓����~���̑����Ƃ�艺�Ȃ瓖�����Ă��Ȃ�
    if ((Sphere_position.GetY() + sphere_radius) < (Cylinder_position.GetY()))
    {
        return false;
    }

    // XZ���ʂł͈̔̓`�F�b�N
    MYVECTOR2 vec = Cylinder_position.GetMyVector2XZ() - Sphere_position.GetMyVector2XZ();
    float lengthXZ = vec.Length();
    float range = sphere_radius + cylinder_radius;
    if (lengthXZ > range)
    {
        return false;
    }

    // A��B�������o��
    vec.Normalize();
    vec *= range;
    MYVECTOR3 Out_Cylinder_position = MYVECTOR3(Sphere_position.GetX() + vec.GetX(), Cylinder_position.GetY(), Sphere_position.GetZ() + vec.GetY());
    out_cylinder_position = Out_Cylinder_position.GetFlaot3();
    return true;

    return true;
}

bool Collision::IntersectRayVsModel(
    DirectX::XMFLOAT3 start,
    DirectX::XMFLOAT3 end,
    const ModelComponent* model,
    HitResult& result
)
{
    // ���[���h��Ԃ̃��C�̒���
    MYVECTOR3 world_start = start;
    MYVECTOR3 world_end = end;
    MYVECTOR3 world_ray_vec = world_end - world_start;
    result.distance = world_ray_vec.Length();

    bool hit = false;
    const ModelResource* resource = model->GetResource();
    // �S�Ẵ��b�V������
    for (const ModelResource::Mesh& mesh : resource->GetMeshes())
    {
        // ���b�V���m�[�h�擾
        const ModelComponent::Node& node = model->GetNodes().at(mesh.node_index);

        // ���C�����[���h��Ԃ��烍�[�J����Ԃ֕ύX
        // ���[���h�s��̋t�s��
        MYMATRIX inverse_world_transform = node.world_transform.GetInverse(nullptr);

        // �n�_����I�_�ւ̃x�N�g��
        MYVECTOR3 S = inverse_world_transform.Vector3TransformCoord(world_start);
        MYVECTOR3 E = inverse_world_transform.Vector3TransformCoord(world_end);
        MYVECTOR3 V = (E - S).Normalize();

        // ���C�̒���
        float lay_length = V.Length();

        // �O�p�`(��)�Ƃ̌�������
        const std::vector<ModelResource::Vertex>& vertices = mesh.vertices; // ���_�o�b�t�@
        const std::vector<UINT> indices = mesh.indices;                     // �C���f�b�N�X�o�b�t�@

        int material_index = -1;
        MYVECTOR3 hit_position;
        MYVECTOR3 hit_normal;
        for (const ModelResource::Subset& subset : mesh.subsets)
        {
            for (UINT i = 0; i < subset.index_count; i += 3)
            {
                UINT index = subset.start_index + i;

                // �O�p�`�̒��_�𒊏o
                const ModelResource::Vertex& a = vertices.at(indices.at(index));
                const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
                const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));

                MYVECTOR3 A(a.position);
                MYVECTOR3 B(b.position);
                MYVECTOR3 C(c.position);

                // �O�p�`�̎O�Ӄx�N�g�����Z�o
                MYVECTOR3 AB = B - A;
                MYVECTOR3 BC = C - B;
                MYVECTOR3 CA = A - C;

                // �O�p�`�̖@���x�N�g�����Z�o
                MYVECTOR3 N = AB.Cross(BC);

                // ���ς̌��ʂ��v���X�Ȃ痠����
                float dot = N.Dot(V);
                if (0.0f <= dot) continue;

                // ���C�ƕ��ʂ̌�������
                MYVECTOR3 SA = A - S;
                float x = (N.Dot(SA)) / dot;
                if (x < 0.f || x > lay_length) continue;     // ��_�܂ł̋��������܂łɌv�Z�����ŋߋ������傫���Ƃ��̓X�L�b�v

                // �O�p�`�ƃ��C�̌�_
                MYVECTOR3 P = S + V * x;

                // ��_���O�p�`�̓����ɂ��邩����
                // 1��
                MYVECTOR3 PA = A - P;
                MYVECTOR3 Cross1 = PA.Cross(AB);
                float dot1 = Cross1.Dot(N);
                if (dot1 < 0.0f)continue;

                // 2��
                MYVECTOR3 PB = B - P;
                MYVECTOR3 Cross2 = PB.Cross(BC);
                float dot2 = Cross2.Dot(N);
                if (dot2 < 0.0f)continue;

                // 3��
                MYVECTOR3 PC = C - P;
                MYVECTOR3 Cross3 = PC.Cross(CA);
                float dot3 = Cross3.Dot(N);
                if (dot3 < 0.0f)continue;

                // �ŒZ�������X�V
                lay_length = x;

                // ��_�Ɩ@�����X�V
                hit_position = P;
                hit_normal = N;
                material_index = subset.material_index;
            }
        }
        if (material_index >= 0)
        {
            // ���[�J�����W���烏�[���h��Ԃ֕ϊ�
            MYVECTOR3 world_positon = node.world_transform.Vector3TransformCoord(hit_position);

            float distance = (world_positon - world_start).Length();

            // �q�b�g���ۑ�
            if (result.distance > distance)
            {
                MYVECTOR3 world_normal = node.world_transform.Vector3TransformNormal(hit_normal);

                result.distance = distance;
                result.material_index = material_index;
                result.position = world_positon.GetFlaot3();
                result.normal = world_normal.GetFlaot3();
                hit = true;
            }
        }
    }
    return hit;
}
