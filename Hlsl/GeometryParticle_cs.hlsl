#include "GeometryParticle.hlsli"
#include "EasingFunctions.hlsli"
#include "./ParticleDisp.h"

// 入力バッファ(構造化バッファ。読み込み専用)
// 入力情報(前フレームの情報)
StructuredBuffer<InputGp> Input : register(t0);
// 入力情報(前フレームの情報。CPUで共有する)
RWStructuredBuffer<CPUGPUBuffer> Input2 : register(u0);

// 出力情報
StructuredBuffer<InputGp> Result : register(t1);
// 出力情報(CPUで共有する)
RWStructuredBuffer<CPUGPUBuffer> Result2 : register(u1);

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
    
    // 稼働していなければ処理しない
    if (!Input2[node].is_busy) return;
    
    // 前フレームの情報をコピーする
    Result[node] = Input[node];

    Result[node].timer -= 1.0f;
    if (Result[node].timer < 0) Result[node].timer = 0;
    
    const float t = (float) ((timer_max - Result[node].timer)) / (float) (timer_max);
    // 透明度の補間
    Result[node].alpha = FadeInOut(t);
    // 拡大率の補間
    Result[node].scale.x = EaseOutQuadInRange(f_scale.x, e_scale.x, (t));
    Result[node].scale.y = EaseOutQuadInRange(f_scale.y, e_scale.y, (t));

    // 稼働中であるか判定する
    Result2[node].is_busy = (0.0f < Result[node].timer) ? 1 : 0;
}