//  ���_���
struct VsIn
{
    float3 position : POSITION;
    float2 size : TEXCOORD;
    float4 color : COLOR;
    float4 param : PARAMETER;
};

struct GsIn
{
    float3 position : POSITION;
    float2 size : TEXCOORD;
    float4 color : COLOR;
    float4 param : PARAMETER;
};

struct PsIn
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

cbuffer GeometryParticleData : register(b0)
{
    float2 Size; //  �p�[�e�B�N���̑傫��
    float2 dummy;
};

cbuffer SceneConstantBuffer : register(b1)
{
    row_major float4x4 view_projection;
    row_major float4x4 view_matrix;
    row_major float4x4 projection_matrix;
};
