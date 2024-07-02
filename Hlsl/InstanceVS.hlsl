
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
