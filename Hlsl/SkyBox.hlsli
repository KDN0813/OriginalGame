struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float4 world_position : WORLD_POSITION;
};

cbuffer SCENE_CONSTANT_BUFFER : register(b1)
{
    row_major float4x4 view_projection;
    float4 camera_position;
    row_major float4x4 inverse_view_projection;
};