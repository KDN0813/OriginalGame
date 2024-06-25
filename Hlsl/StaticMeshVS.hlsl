#include "StaticMesh.hlsli"

VS_OUT main(float4 position : POSITION, float2 texcoord : TEXCOORD)
{
    VS_OUT vout;
    vout.position   = mul(position, mul(world, view_projection));
    vout.color      = material_color;
    vout.texcoord   = texcoord;
    
    return vout;
}