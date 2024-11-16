#include "GeometryParticle.hlsli"

StructuredBuffer<InputGp> Input : register(t0);

GsIn main(uint index : SV_VertexID)
{
    GsIn output = (GsIn) 0;
    output.position = Input[index].position;
    output.color = float4(1.0f, 1.0f, 1.0f, Input[index].alpha);
    output.size = default_size;
	
    output.param.rot = Input[index].rot;
    output.param.scale = Input[index].scale;
    return output;
}
