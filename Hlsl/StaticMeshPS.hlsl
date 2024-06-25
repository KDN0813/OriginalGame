#include "StaticMesh.hlsli"

Texture2D color_map : register(t0);
SamplerState sampler_states[3] : register(s0);
#define POINT 0
#define LINER 1
#define ANISTROPIC 2

float4 main(VS_OUT pin) : SV_TARGET
{
    return color_map.Sample(sampler_states[POINT], pin.texcoord) * pin.color;
}