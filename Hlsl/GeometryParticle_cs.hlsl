#include "GeometryParticle.hlsli"
#include "EasingFunctions.hlsli"

#define PERTICLES_COMP_NO  1  //GPU側のスレッド数

// 入力バッファ(構造化バッファ。読み込み専用)
// 前回の値
StructuredBuffer<ParticleData> Input : register(t0);

// 初期化用パラメータ
StructuredBuffer<ParticleData> Input2 : register(t1);
// 出力バッファ(構造化バッファ。読み書き可能)
RWStructuredBuffer<ParticleData> Result : register(u0);

//スレッドの数
#define TH_X PERTICLES_COMP_NO
#define TH_Y 1
#define TH_Z 1

// シェーダ関数
[numthreads(TH_X, TH_Y, TH_Z)]   // スレッド グループのスレッド数
void main(uint3 Gid : SV_GroupID, //グループID　ディスパッチ側で指定
	uint3 GTid : SV_GroupThreadID //スレッドID　ここで指定
)
{
    uint x = GTid.x;
    uint y = Gid.x;
    int node = TH_X * y + x;
    
    if (0.0 <= Input2[node].type)
    {
        Result[node] = Input2[node];
    }
    else
    {
        Result[node] = Input[node];   
    }

    --Result[node].timer;;

    const float t = (Result[node].timer_max - Result[node].timer) / Result[node].timer_max;
    // 透明度の補間
    Result[node].alpha = lerp(0.0f, 1.0f, t);
    // 拡大率の補間
    Result[node].scale.x = EaseOutQuadInRange(Result[node].f_scale.x, Result[node].e_scale.x, (t));
    Result[node].scale.y = EaseOutQuadInRange(Result[node].f_scale.y, Result[node].e_scale.y, (t));
}