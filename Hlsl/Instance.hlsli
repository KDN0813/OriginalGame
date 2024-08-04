// �y���̓f�[�^�z
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

// �y�o�̓f�[�^�z
struct VsOut
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
};


// �y�\���́z
struct WorldTransform
{
    row_major float4x4 transform;
};

struct BoneTransform
{
    row_major float4x4 transform;
};


// �y�萔�o�b�t�@�z
cbuffer SceneConstantBuffer : register(b0)
{
    row_major float4x4 view_projection;
};

cbuffer SubsetConstantBuffer : register(b1)
{
    float4 material_color;
};