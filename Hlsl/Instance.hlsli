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

#define MAX_INSTANCES 512
cbuffer InstancingMeshConstantBuffer : register(b1)
{
    row_major float4x4 worldTransform[MAX_INSTANCES];
};

cbuffer SubsetConstantBuffer : register(b2)
{
    float4 material_color;
};