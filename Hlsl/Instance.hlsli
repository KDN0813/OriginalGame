#include "SceneConstantBuffer.hlsli"

// �y���̓f�[�^�z
struct VsIn
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
    float4 bone_weights : WEIGHTS;
    uint4 bone_indices : BONES;
    uint instance_id : SV_INSTANCEID;
};

// �y�o�̓f�[�^�z
struct VsOut
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
};


// �y�\���́z
struct InstanceData
{
    uint animation_start_offset;    // �o�b�t�@���Ŏg�p����A�j���[�V�����̊J�n�ʒu�������I�t�Z�b�g�l
    uint frame;                     // ���݂̃t���[��
    row_major float4x4 world_transform;
};
struct BoneTransform
{
    row_major float4x4 bone_transform;
};


// �y�萔�o�b�t�@�z
cbuffer SubsetConstantBuffer : register(b1)
{
    float4 material_color;
};
// �C���X�^���V���O�`��Ŏg�p���鋤�ʂ̒萔
cbuffer CommonDataConstantBuffer : register(b2)
{
    uint bone_transform_count; // 1��̕`��Ŏg�p����{�[���g�����X�t�H�[���̐�
    uint3 commondata_dummy;
};
// ���b�V�����Őݒ肷��l
cbuffer MeshConstantBuffer : register(b3)
{
    uint mesh_offset;            // �o�b�t�@���Ń��b�V���̊J�n�ʒu�������I�t�Z�b�g�l
    uint3 mesh_dummy;
}