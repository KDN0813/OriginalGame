#include "StaticMesh.hlsli"

VsOut main(
	float4 position : POSITION,
	float3 normal : NORMAL,
	float3 tangent : TANGENT,
	float2 texcoord : TEXCOORD,
	float4 color : COLOR,
	float4 bone_weights : WEIGHTS,
	uint4 bone_indices : BONES
)
{
    float3 p = { 0, 0, 0 };
    float3 n = { 0, 0, 0 };
    for (int i = 0; i < 4; i++)
    {
        p += (bone_weights[i] * mul(position, bone_transforms[bone_indices[i]])).xyz;
        n += (bone_weights[i] * mul(float4(normal.xyz, 0), bone_transforms[bone_indices[i]])).xyz;
    }
    
    VsOut vout;
    vout.position = mul(float4(p, 1.0f), view_projection);
    //vout.position   = mul(position, mul(world, view_projection));
    vout.color      = material_color;
    vout.texcoord   = texcoord;
    
    return vout;
}