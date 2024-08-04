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
struct WorldTransform
{
    row_major float4x4 transform;
};

struct BoneTransform
{
    row_major float4x4 transform;
};

//== BoneTransformTextureを取り出すのに必要なパラメータ ==
struct BoneTransformData
{
    uint bone_size; // 1回の描画で使用するボーントランスフォームの数
    uint mesh_first_bone_index; // 描画するメッシュの先頭のボーントランスフォームのインデックス
    uint animation_first_bone_index; // 使うアニメーションの先頭のボーントランスフォームのインデックス
    uint frame; // 現在のフレーム
};


// 【定数バッファ】
cbuffer SceneConstantBuffer : register(b0)
{
    row_major float4x4 view_projection;
};

cbuffer SubsetConstantBuffer : register(b1)
{
    float4 material_color;
};