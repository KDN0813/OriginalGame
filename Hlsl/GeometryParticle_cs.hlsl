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
    
    // 前フレームの情報をコピーする
    Result2[node] = Input2[node];
    
    // 稼働していなければ処理しない
    if (!Input2[node].is_busy)
    {
        Result[node].alpha = 0.0f;
        return;
    }

    // 計算用変数
    // Resultに直接加算すると中間結果が取得できないため、
    // 一度変数に代入してから計算する
    float3 color = Input[node].color;
    float3 position = Input[node].position;
    float3 velocity = Input[node].velocity;
    float2 scale = Input[node].scale;
    float rot = Input[node].rot;
    float alpha = Input[node].alpha;
    float lifetimer = Input[node].lifetimer;
    
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
                    position = Input2[node].initial_position;
                    velocity = Input2[node].velocity;
                    rot = Input2[node].rot;
                    alpha = 0.0f;
                    scale = Input2[node].initial_scale;
                    lifetimer = Input2[node].initial_lifetime;
                    // CPU共有データの設定
                    Result2[node].step = 1;
                    break;
                case 1: // 更新
            
                    lifetimer = max(0.0f, lifetimer - elapsed_time);
                    t = (Input2[node].initial_lifetime - lifetimer) / Input2[node].initial_lifetime;
            
                    // 透明度の補間
                    alpha = FadeInOut(t);
                    // 拡大率の補間
                    scale.x = EaseOutQuadInRange(Input2[node].f_scale.x, Input2[node].e_scale.x, (t));
                    scale.y = EaseOutQuadInRange(Input2[node].f_scale.y, Input2[node].e_scale.y, (t));
            
                    if (0.0f < lifetimer) break;
                    // 寿命が0以下になった実行
                    Result2[node].is_busy = 0;
                    alpha = 0.0f;
                    
                    break;
            }
        
            break;   // 斬撃エフェクト
        
        case EFFECT_HIT: // ヒットエフェクト
            switch (Input2[node].step)
            {
                case 0: // 初期設定
                
                    // GPU専用データの設定
                    color = Input2[node].color;
                    position = Input2[node].initial_position;
                    velocity = Input2[node].velocity;
                    rot = Input2[node].rot;
                    alpha = 1.0f;
                    scale = Input2[node].initial_scale;
                    lifetimer = Input2[node].initial_lifetime;
                    // CPU共有データの設定
                    Result2[node].step = 1;
                    break;
                case 1: // 更新

                    lifetimer = max(0.0f, lifetimer - elapsed_time);
                    t = (Input2[node].initial_lifetime - lifetimer) / Input2[node].initial_lifetime;
            
                    // 角度更新
                    rot = NormalizeRadiansIfOutOfRange(rot + Input2[node].rot_speed * elapsed_time);
            
                    // 位置更新
                    position += velocity * elapsed_time;
                    // 速度更新
                    velocity += Input2[node].acceleration * elapsed_time;
                    // 透明度更新
                    alpha = EaseInQuartLerp(1.0f, 0.0f, t);
            
                    if (0.0f < lifetimer)
                        break;
                    // パーティクルが地面まで移動したらor寿命が尽きたら実行
                    Result2[node].is_busy = 0;
                    alpha = 0.0f;
            
                    break;
            }
        
            break; // 落下斬撃エフェクト
    }
    
    // Resultに計算結果を代入
    Result[node].color = color;
    Result[node].position = position;
    Result[node].velocity = velocity;
    Result[node].scale = scale;
    Result[node].rot = rot;
    Result[node].alpha = alpha;
    Result[node].lifetimer = lifetimer;
}