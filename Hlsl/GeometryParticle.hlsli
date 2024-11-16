
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

// シェーダの入力データ定義（シェーダー内ループ用)
// CPUで共有しないデータ
struct InputGp
{
    float3 position;// 描画位置
    float2 scale;   // 拡大率
    float alpha;    // 透明度
    int timer;      // 生存時間(最大値は定数で持つ)
};

// CPUで共有するデータ
struct CPUGPUBuffer
{
    float3 position;// 発生位置
    float rot;      // 角度
    int step;
    int is_busy;    // 要素が稼働中であるか
};

// パーティク共通の定数
cbuffer ParticleCommonConstant : register(b1)
{
    float2 default_size;    // 通常画像サイズ
    float2 f_scale;         // 拡大率(開始)
    float2 e_scale;         // 拡大率(終了)
    int timer_max;          // 生存時間
};