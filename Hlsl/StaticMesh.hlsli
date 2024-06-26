struct VsOut
{
    float4 position : SV_POSITION;      // 頂点座標
    float4 color : COLOR;               // 色
    float2 texcoord : TEXCOORD;         // テクスチャ
};

cbuffer SceneConstantBuffer : register(b3)
{
    row_major float4x4 view_projection; // ビュー行列
};
#define MAX_BONES 128
cbuffer MeshConstantBuffer : register(b1)
{
    row_major float4x4 bone_transforms[MAX_BONES];
};
cbuffer SubsetConstantBuffer : register(b0)
{
    float4 material_color;              // マテリアルのカラー
};