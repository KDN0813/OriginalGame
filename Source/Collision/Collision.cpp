#include "Collision.h"
#include "Model/ModelResource.h"

// ���Ƌ��̌�������[05]
bool Collision::IntersectSphareVsSphere(
    const DirectX::XMFLOAT3& positionA
    , float radiusA
    , const DirectX::XMFLOAT3& positionB
    , float radiusB
    , DirectX::XMFLOAT3& out_positionB)
{
    // A����B�̒P�ʃx�N�g�����Z�o[05]
    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR Vec       = DirectX::XMVectorSubtract(PositionB, PositionA);
    //DirectX::XMVECTOR LengthSp  = DirectX::XMVector3Length(Vec);
    DirectX::XMVECTOR LengthSp  = DirectX::XMVector3LengthSq(Vec);
    // Sp�����Ă���Ɓ�����v�Z������Ȃ�

    float lengthSP;
    DirectX::XMStoreFloat(&lengthSP, LengthSp);

    // ��������[05]
    float range = radiusA + radiusB;
    // LengthSp����悵���l�Ȃ̂�range����悷��
    if (lengthSP > range * range)
    {
        return false;
    }
    float Y;
    Y = DirectX::XMVectorGetY(Vec);

    // A��B�������o��
    DirectX::XMVECTOR OutPositionB = DirectX::XMVector3Normalize(Vec);
    OutPositionB = DirectX::XMVectorScale(OutPositionB, range);
    OutPositionB = DirectX::XMVectorAdd(PositionA, OutPositionB);


    DirectX::XMStoreFloat3(&out_positionB, OutPositionB);

    return true;
}

// �~���Ɖ~���̌�������[06]
bool Collision::IntersectCylinderVsCylinder(
    const DirectX::XMFLOAT3& positionA,
    float radiusA,
    float heightA,
    const DirectX::XMFLOAT3& positionB,
    float radiusB,
    float heightB,
    DirectX::XMFLOAT3& out_positionB
)
{
    // A�̑����Ƃ�B�̓��̏�ɓ������Ă��Ȃ�[06]
    if ((positionA.y) > (positionB.y + heightB))
    {
        return false;
    }

    // A�̓���B�̑����Ƃ�艺�Ȃ瓖�����Ă��Ȃ�[06]
    if ((positionA.y + heightB) < (positionB.y))
    {
        return false;
    }

    // XZ���ʂł͈̔̓`�F�b�N[06]
    DirectX::XMFLOAT2 vec = { positionB.x - positionA.x,positionB.z - positionA.z };
    float lengthXZ = sqrtf(vec.x * vec.x) + (vec.y * vec.y);
    float range = radiusA + radiusB;
    if (lengthXZ > range)
    {
        return false;
    }

    // A��B�������o��
    vec = { vec.x / lengthXZ,vec.y / lengthXZ };
    out_positionB.x = positionA.x + vec.x * range;
    out_positionB.y = positionB.y;
    out_positionB.z = positionA.z + vec.y * range;
    return true;
}

// ���Ɖ~���̌�������[08]
bool Collision::IntersectSphereVsCylinder(
    const DirectX::XMFLOAT3& sphere_position
    , float sphere_radius
    , const DirectX::XMFLOAT3& cylinder_position
    , float cylinder_radius
    , float cylinder_height
    , DirectX::XMFLOAT3& out_cylinder_position
)
{
    // ���̑����Ƃ��~���̓��̏�ɓ������Ă��Ȃ�[06]
    if ((sphere_position.y - sphere_radius) > (cylinder_position.y + cylinder_height))
    {
        return false;
    }

    // ���̓����~���̑����Ƃ�艺�Ȃ瓖�����Ă��Ȃ�[06]
    if ((sphere_position.y + sphere_radius) < (cylinder_position.y))
    {
        return false;
    }

    // XZ���ʂł͈̔̓`�F�b�N[06]
    DirectX::XMFLOAT2 vec = { cylinder_position.x - sphere_position.x,cylinder_position.z - sphere_position.z };
    float lengthXZ = sqrtf(vec.x * vec.x) + (vec.y * vec.y);
    float range = sphere_radius + cylinder_radius;
    if (lengthXZ > range)
    {
        return false;
    }

    // A��B�������o��
    vec = { vec.x / lengthXZ,vec.y / lengthXZ };
    out_cylinder_position.x = sphere_position.x + vec.x * range;
    out_cylinder_position.y = cylinder_position.y;
    out_cylinder_position.z = sphere_position.z + vec.y * range;

    return true;
}

bool Collision::IntersectRayVsModel(
    const DirectX::XMFLOAT3& start,
    const DirectX::XMFLOAT3& end,
    const ModelComponent* model,
    HitResult& result
)
{
    DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
    DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
    DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

    // ���[���h��Ԃ̃��C�̒���
    DirectX::XMStoreFloat(&result.distance, WorldRayLength);

    bool hit = false;
    const ModelResource* resource = model->GetResource();
    // �S�Ẵ��b�V������
    for (const ModelResource::Mesh& mesh : resource->GetMeshes())
    {
        // ���b�V���m�[�h�擾
        const ModelComponent::Node& node = model->GetNodes().at(mesh.node_index);

        // ���C�����[���h��Ԃ��烍�[�J����Ԃ֕ύX
        DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.world_transform);
        DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);     // ���[���h�s��̋t�s��

        DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);      // WorldStart�����b�V���̃��[�J���s��ɍ��킹��3D�x�N�g���ɕϊ�
        DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);        // WorldEnd�����b�V���̃��[�J���s��ɍ��킹��3D�x�N�g���ɕϊ�
        DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
        DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);                                          // S����E�܂ł̃x�N�g���̒P�ʃx�N�g��
        DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);

        // ���C�̒���
        float neart;
        DirectX::XMStoreFloat(&neart, Length);

        // �O�p�`(��)�Ƃ̌�������
        const std::vector<ModelResource::Vertex>& vertices = mesh.vertices; // ���_�o�b�t�@
        const std::vector<UINT> indices = mesh.indices;                     // �C���f�b�N�X�o�b�t�@

        int materialIndex = -1;
        DirectX::XMVECTOR HitPosition;
        DirectX::XMVECTOR HitNormal;
        for (const ModelResource::Subset& subset : mesh.subsets)
        {
            for (UINT i = 0; i < subset.index_count; i += 3)
            {
                UINT index = subset.start_index + i;

                // �O�p�`�̒��_�𒊏o
                const ModelResource::Vertex& a = vertices.at(indices.at(index));
                const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
                const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));

                DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
                DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
                DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

                // �O�p�`�̎O�Ӄx�N�g�����Z�o
                DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
                DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
                DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

                // �O�p�`�̖@���x�N�g�����Z�o
                DirectX::XMVECTOR N = DirectX::XMVector3Cross(AB, BC);

                // ���ς̌��ʂ��v���X�Ȃ痠����
                DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(N, V);
                float dot;
                DirectX::XMStoreFloat(&dot, Dot);
                if (0.0f <= dot) continue;

                // ���C�ƕ��ʂ̌�������
                DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, S);
                //DirectX::XMVECTOR X     = DirectX::XMVector3Dot(V, SA);
                DirectX::XMVECTOR X = DirectX::XMVectorDivide(DirectX::XMVector3Dot(N, SA), Dot);
                float x;
                DirectX::XMStoreFloat(&x, X);
                if (x < 0.f || x > neart) continue;     // ��_�܂ł̋��������܂łɌv�Z�����ŋߋ������傫���Ƃ��̓X�L�b�v

                // �O�p�`�ƃ��C�̌�_
                DirectX::XMVECTOR P = DirectX::XMVectorAdd(S, DirectX::XMVectorScale(V, x));
                P = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(V, X), S);

                // ��_���O�p�`�̓����ɂ��邩����
                // 1��
                DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(A, P);
                DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
                DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, N);
                float dot1;
                DirectX::XMStoreFloat(&dot1, Dot1);
                if (dot1 < 0.0f)continue;

                // 2��
                DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(B, P);
                DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, BC);
                DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, N);
                float dot2;
                DirectX::XMStoreFloat(&dot2, Dot2);
                if (dot2 < 0.0f)continue;

                DirectX::XMFLOAT3 s1, s2;
                DirectX::XMStoreFloat3(&s1, PB);
                DirectX::XMStoreFloat3(&s2, Cross2);

                DirectX::XMFLOAT3 t1, t2;
                // 3��
                DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(C, P);
                DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC, CA);
                DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, N);

                DirectX::XMStoreFloat3(&t1, PC);
                DirectX::XMStoreFloat3(&t2, Cross3);

                float dot3;
                DirectX::XMStoreFloat(&dot3, Dot3);
                if (dot3 < 0.0f)continue;

                // �ŒZ�������X�V
                neart = x;

                // ��_�Ɩ@�����X�V
                HitPosition = P;
                HitNormal = N;
                materialIndex = subset.material_index;
            }
        }
        if (materialIndex >= 0)
        {
            // ���[�J�����W���烏�[���h��Ԃ֕ϊ�
            DirectX::XMVECTOR WorldPositon = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);

            DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPositon, WorldStart);
            DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
            float distance;
            DirectX::XMStoreFloat(&distance, WorldCrossLength);

            // �q�b�g���ۑ�
            if (result.distance > distance)
            {
                DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal, WorldTransform);

                result.distance = distance;
                result.material_index = materialIndex;
                DirectX::XMStoreFloat3(&result.position, WorldPositon);
                DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
                hit = true;
            }
        }
    }
    return hit;
}
