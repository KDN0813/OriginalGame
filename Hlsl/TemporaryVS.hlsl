#include "Temporary.hlsli"

VsOut main(VsIn vs_in)
{
	float3 p = { 0, 0, 0 };
	float3 n = { 0, 0, 0 };
	for (int i = 0; i < 4; i++)
	{
        p += (vs_in.bone_weights[i] * mul(vs_in.position, bone_transforms[vs_in.bone_indices[i]])).xyz;
        n += (vs_in.bone_weights[i] * mul(float4(vs_in.normal.xyz, 0), bone_transforms[vs_in.bone_indices[i]])).xyz;
    }

    //p = mul(vs_in.position, bone_transforms[0];
    VsOut vout;
	vout.position = mul(float4(p, 1.0f), view_projection);
	
    float3 N = normalize(n);
    float3 L = normalize(-directional_lights.direction.xyz);
    float d = dot(L, N);
    float power = max(0, d) * 0.5f + 0.5f;
    vout.color.rgb = vs_in.color.rgb * material_color.rgb * power * ambient_color.rgb;
    vout.color.a = vs_in.color.a * material_color.a * 2.0f;
    vout.texcoord = vs_in.texcoord * tile_count;
    return vout;
}
