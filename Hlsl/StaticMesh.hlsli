struct VsOut
{
    float4 position : SV_POSITION;      // ���_���W
    float4 color : COLOR;               // �F
    float2 texcoord : TEXCOORD;         // �e�N�X�`��
};

cbuffer SceneConstantBuffer : register(b3)
{
    row_major float4x4 view_projection; // �r���[�s��
};
#define MAX_BONES 128
cbuffer MeshConstantBuffer : register(b1)
{
    row_major float4x4 bone_transforms[MAX_BONES];
};
cbuffer SubsetConstantBuffer : register(b0)
{
    float4 material_color;              // �}�e���A���̃J���[
};