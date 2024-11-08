#include "GeometryParticle.hlsli"

Texture2D color_map : register(t0);

#define POINT 0
#define LINEAR 1
#define ANISOTOROPIC 2
SamplerState sampler_states[3] : register(s0);

float4 main(PsIn pin) : SV_TARGET0
{
    return color_map.Sample(sampler_states[ANISOTOROPIC], pin.texcoord) * pin.color;
}