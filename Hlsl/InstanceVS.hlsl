
#include "Instance.hlsli"

StructuredBuffer<BoneTransform> bone_transform_texture : register(t1);
StructuredBuffer<InstanceData> instance_data : register(t2);

VsOut main(VsIn vs_in)
{        
    const uint frame = instance_data[vs_in.instance_id].frame;
    const uint start_offset = (bone_transform_count * frame) + mesh_offset + (bone_transform_count * instance_data[vs_in.instance_id].animation_start_offset);
    const uint transform_index[4] =
    {
        start_offset + vs_in.bone_indices[0],
        start_offset + vs_in.bone_indices[1],
        start_offset + vs_in.bone_indices[2],
        start_offset + vs_in.bone_indices[3],
    };
    
    float3 p = { 0, 0, 0 };
    float3 n = { 0, 0, 0 };
    for (int i = 0; i < 4; i++)
    {
        float4x4 bone_transform = bone_transform_texture[transform_index[i]].bone_transform;
        p += (vs_in.bone_weights[i] * mul(vs_in.position, bone_transform)).xyz;
        n += (vs_in.bone_weights[i] * mul(float4(vs_in.normal.xyz, 0), bone_transform)).xyz;
    }
    float4x4 world_transform = instance_data[vs_in.instance_id].world_transform;
    
    VsOut vout;
    vout.position = mul(float4(p, 1.0f), mul(world_transform, view_projection));
    //vout.position = mul(float4(p, 1.0f), view_projection);
	
    //vout.position = mul(mul(vs_in.position, instance_data[vs_in.instance_id].world_transform), view_projection);
	
    vout.color.rgb = vs_in.color.rgb * material_color.rgb;
    vout.color.a = vs_in.color.a * material_color.a;
    vout.texcoord = vs_in.texcoord;
    
#if 0   // デバッグの使用
#if 0   // TODO デバッグの種類切り替え
    vout.color.r = (float) (instance_data[vs_in.instance_id].bone_transform_data.animation_first_bone_index) / 255.0f;
    vout.color.g = 0.0f;
    vout.color.b = 0.0f;
    vout.color.a = 1.0f;
#else
    
    uint debug_frame = 0;
    
    uint debug_transform_index = (bone_transform_count * frame) + mesh_offset;
    
    //float4x4 transform = bone_transform_texture[2].transform;
    float4x4 transform = bone_transform_texture[debug_transform_index].transform;
    vout.color = float4(
        transform._11,
        transform._12,
        transform._13,
        transform._14
    );
#endif    
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