// 行列の分解
void DecomposeMatrix(
const float4x4 m,
out float3 translation,
out float3 rotation,
out float3 scale
)
{
    // 平行移動の抽出
    translation = m[3].xyz;
    
    // スケールの抽出
    scale = float3(length(m[0].xyz), length(m[1].xyz), length(m[2].xyz));
    
    // 回転の抽出
    rotation[0] = m[0].xyz / scale.x; // X軸のスケールを除算
    rotation[1] = m[1].xyz / scale.y; // Y軸のスケールを除算
    rotation[2] = m[2].xyz / scale.z; // Z軸のスケールを除算
}