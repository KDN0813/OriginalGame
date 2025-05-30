#include "Instance.hlsli"

Texture2D diffuse_map : register(t0);
SamplerState diffuse_map_sampler_state : register(s0);

float4 main(VsOut pin) : SV_TARGET
{
    return diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord) * pin.color;
    //return pin.color;
}

