#include "GeometryParticle.hlsli"

Texture2D color_map : register(t0);

SamplerState sampler_states : register(s0);

float4 main(PsIn pin) : SV_TARGET0
{
    return color_map.Sample(sampler_states, pin.texcoord) * pin.color;
}