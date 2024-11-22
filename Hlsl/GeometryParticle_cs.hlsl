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
    
    // �O�t���[���̏����R�s�[����
    Result2[node] = Input2[node];
    
    // �ғ����Ă��Ȃ���Ώ������Ȃ�
    if (!Input2[node].is_busy)
    {
        Result[node].alpha = 0.0f;
        return;
    }

    // �v�Z�p�ϐ�
    // Result�ɒ��ډ��Z����ƒ��Ԍ��ʂ��擾�ł��Ȃ����߁A
    // ��x�ϐ��ɑ�����Ă���v�Z����
    float3 color = Input[node].color;
    float3 position = Input[node].position;
    float3 velocity = Input[node].velocity;
    float2 scale = Input[node].scale;
    float rot = Input[node].rot;
    float alpha = Input[node].alpha;
    float lifetimer = Input[node].lifetimer;
    
    // ��ԗ�
    float t = 0.0f;
    
    switch (Input2[node].type)
    {
        // �e��X�V
        
        case EFFECT_SLASH:  // �a���G�t�F�N�g
            switch (Input2[node].step)
            {
                case 0: // �����ݒ�
                
                    // GPU��p�f�[�^�̐ݒ�
                    color = Input2[node].color;
                    position = Input2[node].initial_position;
                    velocity = Input2[node].velocity;
                    rot = Input2[node].rot;
                    alpha = 0.0f;
                    scale = Input2[node].initial_scale;
                    lifetimer = Input2[node].initial_lifetime;
                    // CPU���L�f�[�^�̐ݒ�
                    Result2[node].step = 1;
                    break;
                case 1: // �X�V
            
                    lifetimer = max(0.0f, lifetimer - elapsed_time);
                    t = (Input2[node].initial_lifetime - lifetimer) / Input2[node].initial_lifetime;
            
                    // �����x�̕��
                    alpha = FadeInOut(t);
                    // �g�嗦�̕��
                    scale.x = EaseOutQuadInRange(Input2[node].f_scale.x, Input2[node].e_scale.x, (t));
                    scale.y = EaseOutQuadInRange(Input2[node].f_scale.y, Input2[node].e_scale.y, (t));
            
                    if (0.0f < lifetimer) break;
                    // ������0�ȉ��ɂȂ������s
                    Result2[node].is_busy = 0;
                    alpha = 0.0f;
                    
                    break;
            }
        
            break;   // �a���G�t�F�N�g
        
        case EFFECT_HIT: // �q�b�g�G�t�F�N�g
            switch (Input2[node].step)
            {
                case 0: // �����ݒ�
                
                    // GPU��p�f�[�^�̐ݒ�
                    color = Input2[node].color;
                    position = Input2[node].initial_position;
                    velocity = Input2[node].velocity;
                    rot = Input2[node].rot;
                    alpha = 1.0f;
                    scale = Input2[node].initial_scale;
                    lifetimer = Input2[node].initial_lifetime;
                    // CPU���L�f�[�^�̐ݒ�
                    Result2[node].step = 1;
                    break;
                case 1: // �X�V

                    lifetimer = max(0.0f, lifetimer - elapsed_time);
                    t = (Input2[node].initial_lifetime - lifetimer) / Input2[node].initial_lifetime;
            
                    // �p�x�X�V
                    rot = NormalizeRadiansIfOutOfRange(rot + Input2[node].rot_speed * elapsed_time);
            
                    // �ʒu�X�V
                    position += velocity * elapsed_time;
                    // ���x�X�V
                    velocity += Input2[node].acceleration * elapsed_time;
                    // �����x�X�V
                    alpha = EaseInQuartLerp(1.0f, 0.0f, t);
            
                    if (0.0f < lifetimer)
                        break;
                    // �p�[�e�B�N�����n�ʂ܂ňړ�������or�������s��������s
                    Result2[node].is_busy = 0;
                    alpha = 0.0f;
            
                    break;
            }
        
            break; // �����a���G�t�F�N�g
    }
    
    // Result�Ɍv�Z���ʂ���
    Result[node].color = color;
    Result[node].position = position;
    Result[node].velocity = velocity;
    Result[node].scale = scale;
    Result[node].rot = rot;
    Result[node].alpha = alpha;
    Result[node].lifetimer = lifetimer;
}