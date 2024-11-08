//#include "SceneConstantBuffer.hlsli"

//  頂点情報
struct VS_IN
{
    float3 position : POSITION;
    float2 size : TEXCOORD;
    float4 color : COLOR;
    float4 param : PARAMETER;
};

struct GS_IN
{
    float3 position : POSITION;
    float2 size : TEXCOORD;
    float4 color : COLOR;
    float4 param : PARAMETER;
};

struct PS_IN
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

cbuffer GEOMETRY_PARTICLE_DATA : register(b0)
{
    float2 Size; //  パーティクルの大きさ
    float2 dummy;
};

cbuffer SCENE_CONSTANT_BUFFER : register(b1)
{
    row_major float4x4 view_projection;
    float4 options;
    float4 camera_position;
    row_major float4x4 inverse_view_projection;
    row_major float4x4 view_matrix;
    row_major float4x4 projection_matrix;
};
