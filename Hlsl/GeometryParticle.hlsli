
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
    float4 color : COLOR;
    ParticleParam param;
};

struct GsIn
{
    float3 position : POSITION;
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
    float3 color;
    float3 position; // 描画位置
    float3 velocity; // 移動速度
    float2 scale;   // 拡大率
    float rot;      // 角度
    float alpha;    // 透明度
    float lifetimer;// 生存時間
};

// CPUで共有するデータ
struct CPUGPUBuffer
{
    float3 initial_position;// 初期位置
    float3 forward;         // 前方方向
    float3 velocity;        // 移動速度
    float3 acceleration;    // 加速度
    float2 initial_scale;   // 初期拡大率
    float2 f_scale;         // 拡大率(開始)
    float2 e_scale;         // 拡大率(終了)
    float3 color;           // 初期色   
    float3 f_color;         // 初期色(開始)   
    float3 e_color;         // 初期色(終了)   
    float rot;              // 初期角度
    float rot_speed;        // 回転速度
    float initial_lifetime; // 初期生存時間
    int type;               // エフェクトの種類
    int step;
    int is_busy;            // 要素が稼働中であるか
};

// パーティク共通の定数
cbuffer ParticleCommonConstant : register(b1)
{
    float elapsed_time;     // 経過時間
    float dummy[3];
};

// ラジアン値を [0, 2π) の範囲に外なら正規化する関数
float NormalizeRadiansIfOutOfRange(float radians)
{
    const float two_pi = 6.28318530718; // 2π
    if (radians < 0 || radians >= two_pi)
    {
        // 範囲外の場合にのみ正規化を行う
        radians = radians - floor(radians / two_pi) * two_pi;
    }
    return radians;
}