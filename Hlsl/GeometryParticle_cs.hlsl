#include "GeometryParticle.hlsli"
#include "EasingFunctions.hlsli"
#include "./ParticleDisp.h"

// ���̓o�b�t�@(�\�����o�b�t�@�B�ǂݍ��ݐ�p)
// ���͏��(�O�t���[���̏��)
StructuredBuffer<InputGp> Input : register(t0);
// ���͏��(�O�t���[���̏��BCPU�ŋ��L����)
StructuredBuffer<CPUGPUBuffer> Input2 : register(t1);

// �o�͏��
RWStructuredBuffer<InputGp> Result : register(u0);
// �o�͏��(CPU�ŋ��L����)
RWStructuredBuffer<CPUGPUBuffer> Result2 : register(u1);

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
    
    // �ғ����Ă��Ȃ���Ώ������Ȃ�
    if (!Input2[node].is_busy)
        return;
    
    // �O�t���[���̏����R�s�[����
    Result[node] = Input[node];

    switch (Input2[node].step)
    {
        case 0: // �����ݒ�
        
            // GPU��p�f�[�^�̐ݒ�
            //Result[node].position = Result2[node].position;
            Result[node].alpha = 0.0f;
            Result[node].scale = f_scale;
            Result[node].timer = timer_max;
            // CPU���L�f�[�^�̐ݒ�
            Result2[node].step = 1;
            break;
        case 1: // �X�V
        
            Result[node].timer = min(0.0f, Result[node].timer - 1.0f); // �����X�V
    
            const float t = (float) ((timer_max - Result[node].timer)) / (float) (timer_max);
            // �����x�̕��
            Result[node].alpha = FadeInOut(t);
            // �g�嗦�̕��
            Result[node].scale.x = EaseOutQuadInRange(f_scale.x, e_scale.x, (t));
            Result[node].scale.y = EaseOutQuadInRange(f_scale.y, e_scale.y, (t));

            // �ғ����ł��邩���肷��
            Result2[node].is_busy = (0.0f < Result[node].timer) ? 1 : 0;
            break;
    }
}