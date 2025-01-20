#include "GeometryParticle.hlsli"

StructuredBuffer<InputGp> Input : register(t0);

GsIn main(uint index : SV_VertexID)
{
    GsIn output = (GsIn) 0;
    output.position = Input[index].position;
    output.color = float4(Input[index].color, Input[index].alpha);
	
    output.param.rot = Input[index].rot;
    output.param.scale = Input[index].scale;
    output.param.texture_no = 2;
    return output;
}
