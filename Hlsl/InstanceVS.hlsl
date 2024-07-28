
#include "Instance.hlsli"

VsOut main(VsIn vs_in)
{
    VsOut vout;
    vout.position = mul(mul(vs_in.position, worldTransform[vs_in.instance_id]), view_projection);
	
    vout.color.rgb = vs_in.color.rgb * material_color.rgb;
    vout.color.a = vs_in.color.a * material_color.a;
    vout.texcoord = vs_in.texcoord;

    return vout;
}

/*
	float3 p = { 0, 0, 0 };
	float3 n = { 0, 0, 0 };
	for (int i = 0; i < 4; i++)
	{
        p += (vs_in.bone_weights[i] * mul(vs_in.position, bone_transforms[vs_in.bone_indices[i]])).xyz;
        n += (vs_in.bone_weights[i] * mul(float4(vs_in.normal.xyz, 0), bone_transforms[vs_in.bone_indices[i]])).xyz;
    }

    VsOut vout;
	vout.position = mul(float4(p, 1.0f), view_projection);
	
	//float3 N = normalize(n);
	
    vout.color.rgb = vs_in.color.rgb * material_color.rgb;
    vout.color.a = vs_in.color.a * material_color.a;
    vout.texcoord = vs_in.texcoord;
*/