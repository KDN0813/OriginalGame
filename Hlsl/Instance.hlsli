struct VsInVertex
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
struct VsInInstancing
{
    row_major float4x4 world_transform : W_TRANSFORM;
};
//== BoneTransformTexture�����o���̂ɕK�v�ȃp�����[�^ ==
struct VsInBoneTransformData
{
    uint bone_size : BONE_SIZE; // 1��̕`��Ŏg�p����{�[���g�����X�t�H�[���̐�
    uint mesh_first_bone_index : MESH_FIRST_BONE_INDEX; // �`�悷�郁�b�V���̐擪�̃{�[���g�����X�t�H�[���̃C���f�b�N�X
    uint animation_first_bone_index : ANIMATION_FIRST_BONE_INDEX; // �g���A�j���[�V�����̐擪�̃{�[���g�����X�t�H�[���̃C���f�b�N�X
    uint frame : FRAME; // ���݂̃t���[��
};

struct VsOut
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
};

cbuffer SceneConstantBuffer : register(b0)
{
    row_major float4x4 view_projection;
};

#define MAX_INSTANCES 512
cbuffer InstancingMeshConstantBuffer : register(b1)
{
    row_major float4x4 worldTransform[MAX_INSTANCES];
};

cbuffer SubsetConstantBuffer : register(b2)
{
    float4 material_color;
};

struct BoneTransform
{
    row_major float4x4 transform;
};