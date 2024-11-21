
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
    float4 color : COLOR;
    ParticleParam param;
};

struct GsIn
{
    float3 position : POSITION;
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
    float3 color;
    float3 position; // �`��ʒu
    float3 direction;// �ړ�����
    float3 velocity; // �ړ����x
    float2 scale;   // �g�嗦
    float rot;      // �p�x
    float alpha;    // �����x
    float lifetimer;// ��������
};

// CPU�ŋ��L����f�[�^
struct CPUGPUBuffer
{
    float3 initial_position;// �����ʒu
    float3 direction;       // �ړ�����
    float3 velocity;        // �ړ����x
    float3 acceleration;    // �����x
    float2 initial_scale;   // �����g�嗦
    float2 f_scale;         // �g�嗦(�J�n)
    float2 e_scale;         // �g�嗦(�I��)
    float3 color;           // �����F   
    float rot;              // �����p�x
    float rot_speed;        // ��]���x
    float initial_lifetime; // ������������
    int type;               // �G�t�F�N�g�̎��
    int step;
    int is_busy;            // �v�f���ғ����ł��邩
};

// �p�[�e�B�N���ʂ̒萔
cbuffer ParticleCommonConstant : register(b1)
{
    float elapsed_time;     // �o�ߎ���
    float dummy[3];
};