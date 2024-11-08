#include "GeometryParticle.hlsli"

GsIn main(VsIn vin, uint vid : SV_VertexID)
{
    GsIn output = (GsIn) 0;
    output.position = vin.position;
    output.color = vin.color;
    output.size = vin.size;
	
    output.param = vin.param;
    return output;
}
