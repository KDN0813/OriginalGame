// �s��̕���
void DecomposeMatrix(
const float4x4 m,
out float3 translation,
out float3 rotation,
out float3 scale
)
{
    // ���s�ړ��̒��o
    translation = m[3].xyz;
    
    // �X�P�[���̒��o
    scale = float3(length(m[0].xyz), length(m[1].xyz), length(m[2].xyz));
    
    // ��]�̒��o
    rotation[0] = m[0].xyz / scale.x; // X���̃X�P�[�������Z
    rotation[1] = m[1].xyz / scale.y; // Y���̃X�P�[�������Z
    rotation[2] = m[2].xyz / scale.z; // Z���̃X�P�[�������Z
}