#include "StaticMesh.hlsli"

VS_OUT main(float4 position : POSITIONT,float4 normal : NORMAL)
{
    VS_OUT vout;
    vout.position = mul(position, mul(world, view_projection));
    
    return vout;
}