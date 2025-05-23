#include "Instance.hlsli"

StructuredBuffer<BoneTransform> bone_transform_texture : register(t1);
StructuredBuffer<InstanceData> instance_data : register(t2);

VsOut main(VsIn vs_in)
{        
    // 現在のボーン行列の先頭インデックス計算
    const uint frame = instance_data[vs_in.instance_id].frame;
    const uint start_offset = (bone_transform_count * frame) + mesh_offset + instance_data[vs_in.instance_id].animation_start_offset;
    const uint transform_index[4] =
    {
        start_offset + vs_in.bone_indices[0],
        start_offset + vs_in.bone_indices[1],
        start_offset + vs_in.bone_indices[2],
        start_offset + vs_in.bone_indices[3],
    };
    
    // 前回フレームのボーン行列の先頭インデックス計算
    const uint old_frame = instance_data[vs_in.instance_id].old_frame;
    const uint old_start_offset = (bone_transform_count * old_frame) + mesh_offset + instance_data[vs_in.instance_id].old_animation_start_offset;
    const uint old_transform_index[4] =
    {
        old_start_offset + vs_in.bone_indices[0],
        old_start_offset + vs_in.bone_indices[1],
        old_start_offset + vs_in.bone_indices[2],
        old_start_offset + vs_in.bone_indices[3],
    };
    
    const float animation_blend_rate = instance_data[vs_in.instance_id].animation_blend_rate;
    float3 p = { 0, 0, 0 };
    float3 n = { 0, 0, 0 };
    for (int i = 0; i < 4; i++)
    {
        // ボーン行列の取得
        // 線形補間を行う
        float4x4 bone_transform = lerp(bone_transform_texture[old_transform_index[i]].bone_transform, bone_transform_texture[transform_index[i]].bone_transform, animation_blend_rate);
        //float4x4 bone_transform = bone_transform_texture[old_transform_index[i]].bone_transform;
        
        p += (vs_in.bone_weights[i] * mul(vs_in.position, bone_transform)).xyz;
        n += (vs_in.bone_weights[i] * mul(float4(vs_in.normal.xyz, 0), bone_transform)).xyz;
    }
    float4x4 world_transform = instance_data[vs_in.instance_id].world_transform;
    
    VsOut vout;
    vout.position = mul(float4(p, 1.0f), mul(world_transform, view_projection));
    
    float3 N = normalize(n);
    float3 L = normalize(-directional_lights.direction.xyz);
    float d = dot(L, N);
    float power = max(0, d) * 0.5f + 0.5f;
    
    // ベースカラー取得
    const float4 base_color = instance_data[vs_in.instance_id].base_color;
    
    vout.color.rgb = vs_in.color.rgb * material_color.rgb * power * ambient_color.rgb * base_color.rgb;
    vout.color.a = vs_in.color.a * material_color.a * 2.0f * base_color.a;
    
    vout.texcoord = vs_in.texcoord;
    return vout;
}