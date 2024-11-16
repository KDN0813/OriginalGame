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
    if (!Input2[node].is_busy) return;
    
    // �O�t���[���̏����R�s�[����
    Result2[node] = Input2[node];

    // �v�Z�p�ϐ�
    // Result�ɒ��ډ��Z����ƒ��Ԍ��ʂ��擾�ł��Ȃ����߁A
    // ��x�ϐ��ɑ�����Ă���v�Z����
    float3 position = Input[node].position;
    float2 scale = Input[node].scale;
    float alpha = Input[node].alpha;
    float timer = Input[node].timer;
    
    switch (Input2[node].step)
    {
        case 0: // �����ݒ�
        
            // GPU��p�f�[�^�̐ݒ�
            position = Input2[node].position;
            alpha = 0.0f;
            scale = f_scale;
            timer = timer_max;
            // CPU���L�f�[�^�̐ݒ�
            Result2[node].step = 1;
            break;
        case 1: // �X�V
        
            timer = max(0.0f, timer - 1.0f); // �����X�V
            const float t = (timer_max - timer) / timer_max;
            // �����x�̕��
            alpha = FadeInOut(t);
            // �g�嗦�̕��
            scale.x = EaseOutQuadInRange(f_scale.x, e_scale.x, (t));
            scale.y = EaseOutQuadInRange(f_scale.y, e_scale.y, (t));

            // �ғ����ł��邩���肷��
            Result2[node].is_busy = (0.0f < timer) ? 1 : 0;
            break;
    }
    
    // Result�Ɍv�Z���ʂ���
    Result[node].position = position;
    Result[node].scale = scale;
    Result[node].alpha = alpha;
    Result[node].timer = timer;
}