#include "GeometryParticle.hlsli"

StructuredBuffer<ParticleData> Input : register(t0);

GsIn main(VsIn vin, uint index : SV_VertexID)
{
    GsIn output = (GsIn) 0;
    output.position = Input[index].pos;
    output.color = vin.color;
    output.size.x = Input[index].w;
    output.size.y = Input[index].h;
	
    output.param.rot = Input[index].rot;
    output.param.scale = Input[index].scale;
    return output;
}
