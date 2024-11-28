
#include "Instance.hlsli"

StructuredBuffer<BoneTransform> bone_transform_texture : register(t1);
StructuredBuffer<InstanceData> instance_data : register(t2);

VsOut main(VsIn vs_in)
{
    const uint frame = instance_data[vs_in.instance_id].frame;
    const uint start_offset = (bone_transform_count * frame) + mesh_offset + instance_data[vs_in.instance_id].animation_start_offset;
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
    
    float3 N = normalize(n);
    float3 L = normalize(-light_direction.xyz);
    float d = dot(L, N);
    float power = max(0, d) * 0.5f + 0.5f;
    vout.color.rgb = vs_in.color.rgb * material_color.rgb * power;
    vout.color.a = vs_in.color.a * material_color.a * 2.0f;
    
    vout.texcoord = vs_in.texcoord;
    return vout;
}