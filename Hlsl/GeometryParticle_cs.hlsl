#include "GeometryParticle.hlsli"
#include "EasingFunctions.hlsli"

#define PERTICLES_COMP_NO  1  //GPU���̃X���b�h��

// ���̓o�b�t�@(�\�����o�b�t�@�B�ǂݍ��ݐ�p)
// �O��̒l
StructuredBuffer<ParticleData> Input : register(t0);

// �������p�p�����[�^
StructuredBuffer<ParticleData> Input2 : register(t1);
// �o�̓o�b�t�@(�\�����o�b�t�@�B�ǂݏ����\)
RWStructuredBuffer<ParticleData> Result : register(u0);

//�X���b�h�̐�
#define TH_X PERTICLES_COMP_NO
#define TH_Y 1
#define TH_Z 1

// �V�F�[�_�֐�
[numthreads(TH_X, TH_Y, TH_Z)]   // �X���b�h �O���[�v�̃X���b�h��
void main(uint3 Gid : SV_GroupID, //�O���[�vID�@�f�B�X�p�b�`���Ŏw��
	uint3 GTid : SV_GroupThreadID //�X���b�hID�@�����Ŏw��
)
{
    uint x = GTid.x;
    uint y = Gid.x;
    int node = TH_X * y + x;
    
    if (0.0 <= Input2[node].type)
    {
        Result[node] = Input2[node];
    }
    else
    {
        Result[node] = Input[node];   
    }

    --Result[node].timer;;

    const float t = (Result[node].timer_max - Result[node].timer) / Result[node].timer_max;
    // �����x�̕��
    Result[node].alpha = lerp(0.0f, 1.0f, t);
    // �g�嗦�̕��
    Result[node].scale.x = EaseOutQuadInRange(Result[node].f_scale.x, Result[node].e_scale.x, (t));
    Result[node].scale.y = EaseOutQuadInRange(Result[node].f_scale.y, Result[node].e_scale.y, (t));
}