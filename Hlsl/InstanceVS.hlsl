
#include "Instance.hlsli"

StructuredBuffer<BoneTransform> bone_transform_texture : register(t1);
StructuredBuffer<WorldTransform> w_transform : register(t2);

VsOut main(
    VsInVertex vs_in_vertex
    ,VsInInstancing vs_in_instancing
    , VsInBoneTransformData vs_in_BTData
)
{
    VsOut vout;
    vout.position = mul(mul(vs_in_vertex.position, w_transform[vs_in_vertex.instance_id].transform), view_projection);
	
    vout.color.rgb = vs_in_vertex.color.rgb * material_color.rgb;
    vout.color.a = vs_in_vertex.color.a * material_color.a;
    vout.texcoord = vs_in_vertex.texcoord;
    
    // TODO (デバッグ用)
#if 0
    vout.color.r = (float) (vs_in_BTData.bone_size) / 255.0f;
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
        p += (vs_in_vertex.bone_weights[i] * mul(vs_in_vertex.position, bone_transforms[vs_in_vertex.bone_indices[i]])).xyz;
        n += (vs_in_vertex.bone_weights[i] * mul(float4(vs_in_vertex.normal.xyz, 0), bone_transforms[vs_in_vertex.bone_indices[i]])).xyz;
    }

    VsOut vout;
	vout.position = mul(float4(p, 1.0f), view_projection);
	
	//float3 N = normalize(n);
	
    vout.color.rgb = vs_in_vertex.color.rgb * material_color.rgb;
    vout.color.a = vs_in_vertex.color.a * material_color.a;
    vout.texcoord = vs_in_vertex.texcoord;
*/