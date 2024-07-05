
#include "Instance.hlsli"

StrcturedBuffer<float4x4> boneTransforms : register();

VsOut main(VsIn vs_in)
{
    VsOut vout;
    int uid = vs_in.instance_id * 128;
    vout.position = mul(mul(vs_in.position, boneTransforms[uid + vs_in.bone_indices[0]]), view_projection);
    //vout.position = mul(vs_in.position,view_projection);
	
    vout.color.rgb = vs_in.color.rgb * material_color.rgb;
    vout.color.a = vs_in.color.a * material_color.a;
    vout.texcoord = vs_in.texcoord;

    return vout;
}
