#include "GeometryParticle.hlsli"


// �������p���̓o�b�t�@(�\�����o�b�t�@�B�ǂݍ��ݐ�p)
StructuredBuffer<ParticleData> Input : register(t0);
// �o�̓o�b�t�@(�\�����o�b�t�@�B�ǂݏ����\)
RWStructuredBuffer<ParticleData> Result : register(u0);

//�X���b�h�̐�
#define TH_X PERTICLES_COMP_NO
#define TH_Y 1
#define TH_Z 1

// ���͏����o�̓o�b�t�@�ɒǉ�����֐�
[numthreads(TH_X, TH_Y, TH_Z)] // �X���b�h �O���[�v�̃X���b�h��
void main(uint3 Gid : SV_GroupID, //�O���[�vID�@�f�B�X�p�b�`���Ŏw��
	uint3 GTid : SV_GroupThreadID //�X���b�hID�@�����Ŏw��
)
{

};