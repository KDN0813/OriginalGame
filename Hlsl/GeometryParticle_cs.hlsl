#include "GeometryParticle.hlsli"
#include "EasingFunctions.hlsli"
#include "./ParticleDisp.h"

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
        
    // 現状全てのパーティクル情報をステージングバッファでコピーしているので、
    // 新規入力が入力情報と同じなので、一旦新規入力を入力として扱う
#if 0
    if (0.0 <= Input2[node].type)
    {
        Result[node] = Input2[node];
    }
    else
    {
        Result[node] = Input[node];   
    }
#else
    Result[node] = Input2[node];
#endif

    Result[node].timer -= 1.0f;
    if (Result[node].timer < 0) Result[node].timer = 0;
    
    const float t = (float) ((Result[node].timer_max - Result[node].timer)) / (float) (Result[node].timer_max);
    // 透明度の補間
    Result[node].alpha = FadeInOut(t);
    // 拡大率の補間
    Result[node].scale.x = EaseOutQuadInRange(Result[node].f_scale.x, Result[node].e_scale.x, (t));
    Result[node].scale.y = EaseOutQuadInRange(Result[node].f_scale.y, Result[node].e_scale.y, (t));

    Result[node].type = (Result[node].timer <= 0.0f) ? -1.0f : 1.0f;
}