struct VsIn
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
    float4 bone_weights : WEIGHTS;
    uint4 bone_indices : BONES;
    column_major float4x4 mat : MATRIX; // インスタンスごとに設定される行列
    uint InstanceId : SV_InstanceID; // インスタンスＩＤ
};

struct VsOut
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
};

cbuffer SceneConstantBuffer : register(b0)
{
    row_major float4x4 view_projection;
};

#define MAX_BONES 128
cbuffer MeshConstantBuffer : register(b1)
{
    row_major float4x4 bone_transforms[MAX_BONES];
};

cbuffer SubsetConstantBuffer : register(b2)
{
    float4 material_color;
};