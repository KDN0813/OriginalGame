
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

// �V�F�[�_�̓��̓f�[�^��`�i�V�F�[�_�[�����[�v�p)
// CPU�ŋ��L���Ȃ��f�[�^
struct InputGp
{
    float3 position;// �`��ʒu
    float2 scale;   // �g�嗦
    float alpha;    // �����x
    int timer;      // ��������(�ő�l�͒萔�Ŏ���)
};

// CPU�ŋ��L����f�[�^
struct CPUGPUBuffer
{
    float3 position;// �����ʒu
    float rot;      // �p�x
    int step;
    int is_busy;    // �v�f���ғ����ł��邩
};

// �p�[�e�B�N���ʂ̒萔
cbuffer ParticleCommonConstant : register(b1)
{
    float2 default_size;    // �ʏ�摜�T�C�Y
    float2 f_scale;         // �g�嗦(�J�n)
    float2 e_scale;         // �g�嗦(�I��)
    int timer_max;          // ��������
};