#include "SceneConstantBuffer.hlsli"

// 【入力データ】
struct VsIn
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
    float4 bone_weights : WEIGHTS;
    uint4 bone_indices : BONES;
    uint instance_id : SV_INSTANCEID;
};

// 【出力データ】
struct VsOut
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
};


// 【構造体】
struct InstanceData
{
    uint animation_start_offset;    // バッファ内で使用するアニメーションの開始位置を示すオフセット値
    uint frame;                     // 現在のフレーム
    row_major float4x4 world_transform;
};
struct BoneTransform
{
    row_major float4x4 bone_transform;
};


// 【定数バッファ】
cbuffer SubsetConstantBuffer : register(b1)
{
    float4 material_color;
};
// インスタンシング描画で使用する共通の定数
cbuffer CommonDataConstantBuffer : register(b2)
{
    uint bone_transform_count; // 1回の描画で使用するボーントランスフォームの数
    uint3 commondata_dummy;
};
// メッシュ毎で設定する値
cbuffer MeshConstantBuffer : register(b3)
{
    uint mesh_offset;            // バッファ内でメッシュの開始位置を示すオフセット値
    uint3 mesh_dummy;
}