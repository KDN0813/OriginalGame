
// CSで計算するパーティクル情報
struct ParticleData
{
    float3 pos;         // 描画位置
    float w, h;         // 画像サイズ
    float3 scale;       // 拡大率
    float3 f_scale;     // 拡大率(開始)
    float3 e_scale;     // 拡大率(終了)
    float3 v;           // 移動速度
    float3 a;           // 加速度
    float alpha;        // 透明度
    int timer_max;    // 生存時間(最大値)
    int timer;        // 生存時間
    float rot;          // 角度
};

// パーティクル情報
struct ParticleParam
{
    float rot : ROTATION;
    float2 scale : SCALE;
};

//  頂点情報
struct VsIn
{
    float3 position : POSITION;
    float2 size : TEXCOORD;
    float4 color : COLOR;
    ParticleParam param;
};

struct GsIn
{
    float3 position : POSITION;
    float2 size : TEXCOORD;
    float4 color : COLOR;
    ParticleParam param;
};

struct PsIn
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

// 定数バッファ
cbuffer SceneConstantBuffer : register(b0)
{
    row_major float4x4 view_projection;
    row_major float4x4 view_matrix;
    row_major float4x4 projection_matrix;
};
cbuffer GeometryParticleData : register(b0)
{
    float2 Size; //  パーティクルの大きさ
    float2 dummy;
};