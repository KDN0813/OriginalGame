#include "GeometryParticle.hlsli"
#include "EasingFunctions.hlsli"
#include "./ParticleDisp.h"

// 入力バッファ(構造化バッファ。読み込み専用)
// 入力情報(前フレームの情報)
StructuredBuffer<InputGp> Input : register(t0);
// 入力情報(前フレームの情報。CPUで共有する)
StructuredBuffer<CPUGPUBuffer> Input2 : register(t1);

// 出力情報
RWStructuredBuffer<InputGp> Result : register(u0);
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
    if (!Input2[node].is_busy)
        return;
    
    // 前フレームの情報をコピーする
    Result2[node] = Input2[node];

    // 計算用変数
    // Resultに直接加算すると中間結果が取得できないため、
    // 一度変数に代入してから計算する
    float3 color = Input[node].color;
    float3 position = Input[node].position;
    float2 scale = Input[node].scale;
    float rot = Input[node].rot;
    float alpha = Input[node].alpha;
    float timer = Input[node].timer;
    
    // 補間率
    float t = 0.0f;
    
    switch (Input2[node].type)
    {
        // 各種更新
        
        case EFFECT_SLASH:  // 斬撃エフェクト
            switch (Input2[node].step)
            {
                case 0: // 初期設定
                
                    // GPU専用データの設定
                    color = Input2[node].color;
                    position = Input2[node].position;
                    rot = Input2[node].rot;
                    alpha = 0.0f;
                    scale = f_scale;
                    timer = timer_max;
                    // CPU共有データの設定
                    Result2[node].step = 1;
                    break;
                case 1: // 更新

                    // 稼働中であるか判定する
                    Result2[node].is_busy = (0.0f < timer) ? 1 : 0;
                
                    timer = max(0.0f, timer - elapsed_time);
                    t = (timer_max - timer) / timer_max;
                    // 透明度の補間
                    alpha = FadeInOut(t);
                    // 拡大率の補間
                    scale.x = EaseOutQuadInRange(f_scale.x, e_scale.x, (t));
                    scale.y = EaseOutQuadInRange(f_scale.y, e_scale.y, (t));
                    break;
            }
        
            break;   // 斬撃エフェクト
        
        case EFFECT_FALL_SLASH: // 落下斬撃エフェクト
            switch (Input2[node].step)
            {
                case 0: // 初期設定
                
                    // GPU専用データの設定
                    color = Input2[node].color;
                    position = Input2[node].position;
                    rot = Input2[node].rot;
                    alpha = 0.0f;
                    scale = f_scale;
                    timer = timer_max;
                    // CPU共有データの設定
                    Result2[node].step = 1;
                    break;
                case 1: // 更新

                    // 稼働中であるか判定する
                    Result2[node].is_busy = (0.0f < timer) ? 1 : 0;
                
                    timer = max(0.0f, timer - elapsed_time);
                    t = (timer_max - timer) / timer_max;
                    // 色の変更
                    alpha = FadeInOut(t);
                    color.x = EaseOutQuadInRange(f_scale.x, e_scale.x, (t));
                    color.y = EaseOutQuadInRange(f_scale.y, e_scale.y, (t));
                    color.z = FadeInOut(t);
                    break;
            }
        
            break; // 落下斬撃エフェクト
    }
    
    // Resultに計算結果を代入
    Result[node].color = color;
    Result[node].position = position;
    Result[node].scale = scale;
    Result[node].rot = rot;
    Result[node].alpha = alpha;
    Result[node].timer = timer;
}