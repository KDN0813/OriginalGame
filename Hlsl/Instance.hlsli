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
struct WorldTransform
{
    row_major float4x4 transform;
};

struct BoneTransform
{
    row_major float4x4 transform;
};

//== BoneTransformTexture�����o���̂ɕK�v�ȃp�����[�^ ==
struct BoneTransformData
{
    uint bone_size; // 1��̕`��Ŏg�p����{�[���g�����X�t�H�[���̐�
    uint mesh_first_bone_index; // �`�悷�郁�b�V���̐擪�̃{�[���g�����X�t�H�[���̃C���f�b�N�X
    uint animation_first_bone_index; // �g���A�j���[�V�����̐擪�̃{�[���g�����X�t�H�[���̃C���f�b�N�X
    uint frame; // ���݂̃t���[��
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