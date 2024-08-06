
#include "Instance.hlsli"

StructuredBuffer<BoneTransform> bone_transform_texture : register(t1);
StructuredBuffer<InstanceData> instance_data : register(t2);

VsOut main(VsIn vs_in)
{
    VsOut vout;
    vout.position = mul(mul(vs_in.position, instance_data[vs_in.instance_id].world_transform), view_projection);
	
    vout.color.rgb = vs_in.color.rgb * material_color.rgb;
    vout.color.a = vs_in.color.a * material_color.a;
    vout.texcoord = vs_in.texcoord;
        
    // TODO (デバッグ用)
#if 0
    vout.color.r = (float) (instance_data[vs_in.instance_id].bone_transform_data.animation_first_bone_index) / 255.0f;
    vout.color.g = 0.0f;
    vout.color.b = 0.0f;
    vout.color.a = 1.0f;
#else
    
    float4x4 transform = bone_transform_texture[0].transform;
    vout.color = float4(
        transform._11,
        transform._12,
        transform._13,
        transform._14
    );
#endif    
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