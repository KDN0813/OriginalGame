#include "SkyBox.hlsli"

TextureCube Specular_pmrem : register(t0);
SamplerState samplerstate : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
	// �����x�N�g��
    float3 E = normalize(pin.world_position.xyz - camera_position.xyz);
	
	// mipmap���g���ƕ����̂�mipmap�Ȃ��ɂȂ�悤�ɂ���
    return Specular_pmrem.SampleLevel(samplerstate, E, 0) * pin.color;
}