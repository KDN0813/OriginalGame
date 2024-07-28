#include "Instance.hlsli"

Texture2D diffuse_map : register(t0);
StructuredBuffer<BoneTransform> bone_transform_texture : register(t1);
SamplerState diffuse_map_sampler_state : register(s0);

float4 main(VsOut pin, BoneTransformData BTData) : SV_TARGET
//float4 main(VsOut pin) : SV_TARGET
{
    //return diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord) * pin.color;
    
    float4x4 transform = bone_transform_texture[0].transform;
  
    float4 color =
    {
        transform._11,
        transform._12,
        transform._13,
        transform._14
    };
    
    return color;
}

