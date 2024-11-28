#include "Lights.hlsli"

struct VsIn
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
    float4 bone_weights : WEIGHTS;
    uint4 bone_indices : BONES;
};

struct VsOut
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float4 color    : COLOR;
};

#define MAX_BONES 128
cbuffer MeshConstantBuffer : register(b1)
{
    row_major float4x4 bone_transforms[MAX_BONES];
};

cbuffer SceneConstantBuffer : register(b0)
{
    row_major float4x4 view_projection;
};

cbuffer SubsetConstantBuffer : register(b2)
{
    float4 material_color;
    float tile_count;
    float3 dummy;
};

cbuffer LightConstantBuffer : register(b3)
{
    float4 ambient_color;
    DirectionalLights directional_lights;
    float4 light_dummy;
};