#include "Collision.h"
#include "Model/ModelResource.h"

// 球と球の交差判定
bool Collision::IntersectSphareVsSphere(
    DirectX::XMFLOAT3 positionA
    , float radiusA
    , DirectX::XMFLOAT3 positionB
    , float radiusB
    , DirectX::XMFLOAT3 out_positionB)
{
    // AからBの単位ベクトルを算出
    MYVECTOR3 PositionB = positionB;
    MYVECTOR3 PositionA = positionA;
    MYVECTOR3 vec = PositionB - PositionA;

    // 距離判定[05]
    float range = radiusA + radiusB;
    // LengthSpが二乗した値なのでrangeも二乗する
    if (vec.LengthSq() > range * range)
    {
        return false;
    }

    // AがBを押し出す
    MYVECTOR3 Out_positionB = PositionA + (vec * range);
    out_positionB = Out_positionB.GetFlaot3();

    return true;
}

// 円柱と円柱の交差判定
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

    // Aの足もとがBの頭の上に当たっていない
    if ((PositionA.GetY()) > (PositionB.GetY() + heightB))
    {
        return false;
    }

    // Aの頭がBの足もとより下なら当たっていない
    if ((PositionA.GetY() + heightB) < (PositionB.GetY()))
    {
        return false;
    }

    // XZ平面での範囲チェック[06]
    MYVECTOR2 vec = PositionB.GetMyVector2XZ() - PositionA.GetMyVector2XZ();
    float lengthXZ = vec.Length();
    float range = radiusA + radiusB;
    if (lengthXZ > range)
    {
        return false;
    }

    // AがBを押し出す
    vec.Normalize();
    vec *= range;
    MYVECTOR3 Out_positionB = MYVECTOR3(PositionA.GetX() + vec.GetX(), PositionB.GetY(), PositionA.GetZ() + vec.GetY());
    out_positionB = Out_positionB.GetFlaot3();
    return true;
}

// 球と円柱の交差判定
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
    // 球の足もとが円柱の頭の上に当たっていない
    if ((Sphere_position.GetY() - sphere_radius) > (Cylinder_position.GetY() + cylinder_height))
    {
        return false;
    }

    // 球の頭が円柱の足もとより下なら当たっていない
    if ((Sphere_position.GetY() + sphere_radius) < (Cylinder_position.GetY()))
    {
        return false;
    }

    // XZ平面での範囲チェック
    MYVECTOR2 vec = Cylinder_position.GetMyVector2XZ() - Sphere_position.GetMyVector2XZ();
    float lengthXZ = vec.Length();
    float range = sphere_radius + cylinder_radius;
    if (lengthXZ > range)
    {
        return false;
    }

    // AがBを押し出す
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
    // ワールド空間のレイの長さ
    MYVECTOR3 world_start = start;
    MYVECTOR3 world_end = end;
    MYVECTOR3 world_ray_vec = world_end - world_start;
    result.distance = world_ray_vec.Length();

    bool hit = false;
    const ModelResource* resource = model->GetResource();
    // 全てのメッシュを回す
    for (const ModelResource::Mesh& mesh : resource->GetMeshes())
    {
        // メッシュノード取得
        const ModelComponent::Node& node = model->GetNodes().at(mesh.node_index);

        // レイをワールド空間からローカル空間へ変更
        // ワールド行列の逆行列
        MYMATRIX inverse_world_transform = node.world_transform.GetInverse(nullptr);

        // 始点から終点へのベクトル
        MYVECTOR3 S = inverse_world_transform.Vector3TransformCoord(world_start);
        MYVECTOR3 E = inverse_world_transform.Vector3TransformCoord(world_end);
        MYVECTOR3 V = (E - S).Normalize();

        // レイの長さ
        float lay_length = V.Length();

        // 三角形(面)との交差判定
        const std::vector<ModelResource::Vertex>& vertices = mesh.vertices; // 頂点バッファ
        const std::vector<UINT> indices = mesh.indices;                     // インデックスバッファ

        int material_index = -1;
        MYVECTOR3 hit_position;
        MYVECTOR3 hit_normal;
        for (const ModelResource::Subset& subset : mesh.subsets)
        {
            for (UINT i = 0; i < subset.index_count; i += 3)
            {
                UINT index = subset.start_index + i;

                // 三角形の頂点を抽出
                const ModelResource::Vertex& a = vertices.at(indices.at(index));
                const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
                const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));

                MYVECTOR3 A(a.position);
                MYVECTOR3 B(b.position);
                MYVECTOR3 C(c.position);

                // 三角形の三辺ベクトルを算出
                MYVECTOR3 AB = B - A;
                MYVECTOR3 BC = C - B;
                MYVECTOR3 CA = A - C;

                // 三角形の法線ベクトルを算出
                MYVECTOR3 N = AB.Cross(BC);

                // 内積の結果がプラスなら裏向き
                float dot = N.Dot(V);
                if (0.0f <= dot) continue;

                // レイと平面の交差判定
                MYVECTOR3 SA = A - S;
                float x = (N.Dot(SA)) / dot;
                if (x < 0.f || x > lay_length) continue;     // 交点までの距離が今までに計算した最近距離より大きいときはスキップ

                // 三角形とレイの交点
                MYVECTOR3 P = S + V * x;

                // 交点が三角形の内側にあるか判定
                // 1つめ
                MYVECTOR3 PA = A - P;
                MYVECTOR3 Cross1 = PA.Cross(AB);
                float dot1 = Cross1.Dot(N);
                if (dot1 < 0.0f)continue;

                // 2つめ
                MYVECTOR3 PB = B - P;
                MYVECTOR3 Cross2 = PB.Cross(BC);
                float dot2 = Cross2.Dot(N);
                if (dot2 < 0.0f)continue;

                // 3つめ
                MYVECTOR3 PC = C - P;
                MYVECTOR3 Cross3 = PC.Cross(CA);
                float dot3 = Cross3.Dot(N);
                if (dot3 < 0.0f)continue;

                // 最短距離を更新
                lay_length = x;

                // 交点と法線を更新
                hit_position = P;
                hit_normal = N;
                material_index = subset.material_index;
            }
        }
        if (material_index >= 0)
        {
            // ローカル座標からワールド空間へ変換
            MYVECTOR3 world_positon = node.world_transform.Vector3TransformCoord(hit_position);

            float distance = (world_positon - world_start).Length();

            // ヒット情報保存
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
