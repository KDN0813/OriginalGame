
// パーティクル情報
struct ParticleParam
{
    float rot : ROTATION;
    float2 scale : SCALE;
};

//  頂点情報
struct VsIn
{
    float3 position : POSITION;
    float2 size : TEXCOORD;
    float4 color : COLOR;
    ParticleParam param;
};

struct GsIn
{
    float3 position : POSITION;
    float2 size : TEXCOORD;
    float4 color : COLOR;
    ParticleParam param;
};

struct PsIn
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

// 定数バッファ
cbuffer SceneConstantBuffer : register(b0)
{
    row_major float4x4 view_projection;
    row_major float4x4 view_matrix;
    row_major float4x4 projection_matrix;
};
cbuffer GeometryParticleData : register(b0)
{
    float2 Size; //  パーティクルの大きさ
    float2 dummy;
};