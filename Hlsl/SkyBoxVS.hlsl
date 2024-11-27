#include "SkyBox.hlsli"

VS_OUT main(float4 positon : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD)
{
    VS_OUT vout;
    positon.z = 1.0f;
    vout.position = positon;
    vout.color = color;
    
    // NDC座標系からワールド座標へ変換
    vout.world_position = mul(positon, inverse_view_projection);
    vout.world_position /= vout.world_position.w;

    return vout;
}