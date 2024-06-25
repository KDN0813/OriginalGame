struct VS_OUT
{
    float4 position : SV_POSITION;      // ���_���W
    float4 color : COLOR;               // �F
    float2 texcoord : TEXCOORD;         // �e�N�X�`��
};
cbuffer OBJECT_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 world;           // ���[���h�s��
    float4 material_color;              // �}�e���A���̃J���[
};
cbuffer SCENE_CONSTANT_BUFFER : register(b1)
{
    row_major float4x4 view_projection; // �r���[�s��
    float4 camera_position;             // �J�����̈ʒu
};