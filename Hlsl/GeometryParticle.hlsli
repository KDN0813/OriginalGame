
// CS�Ōv�Z����p�[�e�B�N�����
struct ParticleData
{
    float3 pos;         // �`��ʒu
    float w, h;         // �摜�T�C�Y
    float3 scale;       // �g�嗦
    float3 f_scale;     // �g�嗦(�J�n)
    float3 e_scale;     // �g�嗦(�I��)
    float3 v;           // �ړ����x
    float3 a;           // �����x
    float alpha;        // �����x
    int timer_max;    // ��������(�ő�l)
    int timer;        // ��������
    float rot;          // �p�x
};

// �p�[�e�B�N�����
struct ParticleParam
{
    float rot : ROTATION;
    float2 scale : SCALE;
};

//  ���_���
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

// �萔�o�b�t�@
cbuffer SceneConstantBuffer : register(b0)
{
    row_major float4x4 view_projection;
    row_major float4x4 view_matrix;
    row_major float4x4 projection_matrix;
};
cbuffer GeometryParticleData : register(b0)
{
    float2 Size; //  �p�[�e�B�N���̑傫��
    float2 dummy;
};