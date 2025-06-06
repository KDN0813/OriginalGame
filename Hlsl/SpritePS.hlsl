
#include "sprite.hlsli"

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

float4 main(VsOut pin) : SV_TARGET
{
	return texture0.Sample(sampler0, pin.texcoord) * pin.color;
}

