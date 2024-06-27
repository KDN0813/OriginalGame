
#include "Lambert.hlsli"

// ランバードシェーダーの計算こっちでやってるぽい
// →色の計算をこっちで行ってる
VsOut main(
	float4 position     : POSITION,
	float3 normal : NORMAL,
	float3 tangent : TANGENT,
	float2 texcoord : TEXCOORD,
	float4 color : COLOR,
	float4 bone_weights : WEIGHTS,
	uint4  bone_indices : BONES
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

	//float3 N = normalize(n);
	
	vout.color.rgb = color.rgb * material_color.rgb;
	vout.color.a = color.a * material_color.a;
	vout.texcoord = texcoord;

	return vout;
}
