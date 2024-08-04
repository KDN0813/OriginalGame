// 【入力データ】
struct VsInVertex
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
struct VsInInstancing
{
    row_major float4x4 world_transform : W_TRANSFORM;
};
//== BoneTransformTextureを取り出すのに必要なパラメータ ==
struct VsInBoneTransformData
{
    uint bone_size : BONE_SIZE; // 1回の描画で使用するボーントランスフォームの数
    uint mesh_first_bone_index : MESH_FIRST_BONE_INDEX; // 描画するメッシュの先頭のボーントランスフォームのインデックス
    uint animation_first_bone_index : ANIMATION_FIRST_BONE_INDEX; // 使うアニメーションの先頭のボーントランスフォームのインデックス
    uint frame : FRAME; // 現在のフレーム
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


// 【定数バッファ】
cbuffer SceneConstantBuffer : register(b0)
{
    row_major float4x4 view_projection;
};

cbuffer SubsetConstantBuffer : register(b1)
{
    float4 material_color;
};